receive_encrypted_read(struct TCP_Server_Info *server, struct mid_q_entry **mid,
		       int *num_mids)
{
	char *buf = server->smallbuf;
	struct smb2_transform_hdr *tr_hdr = (struct smb2_transform_hdr *)buf;
	unsigned int npages;
	struct page **pages;
	unsigned int len;
	unsigned int buflen = server->pdu_size;
	int rc;
	int i = 0;
	struct smb2_decrypt_work *dw;

	*num_mids = 1;
	len = min_t(unsigned int, buflen, server->vals->read_rsp_size +
		sizeof(struct smb2_transform_hdr)) - HEADER_SIZE(server) + 1;

	rc = cifs_read_from_socket(server, buf + HEADER_SIZE(server) - 1, len);
	if (rc < 0)
		return rc;
	server->total_read += rc;

	len = le32_to_cpu(tr_hdr->OriginalMessageSize) -
		server->vals->read_rsp_size;
	npages = DIV_ROUND_UP(len, PAGE_SIZE);

	pages = kmalloc_array(npages, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		rc = -ENOMEM;
		goto discard_data;
	}

	for (; i < npages; i++) {
		pages[i] = alloc_page(GFP_KERNEL|__GFP_HIGHMEM);
		if (!pages[i]) {
			rc = -ENOMEM;
			goto discard_data;
		}
	}

	/* read read data into pages */
	rc = read_data_into_pages(server, pages, npages, len);
	if (rc)
		goto free_pages;

	rc = cifs_discard_remaining_data(server);
	if (rc)
		goto free_pages;

	/*
	 * For large reads, offload to different thread for better performance,
	 * use more cores decrypting which can be expensive
	 */

	if ((server->min_offload) && (server->in_flight > 1) &&
	    (server->pdu_size >= server->min_offload)) {
		dw = kmalloc(sizeof(struct smb2_decrypt_work), GFP_KERNEL);
		if (dw == NULL)
			goto non_offloaded_decrypt;

		dw->buf = server->smallbuf;
		server->smallbuf = (char *)cifs_small_buf_get();

		INIT_WORK(&dw->decrypt, smb2_decrypt_offload);

		dw->npages = npages;
		dw->server = server;
		dw->ppages = pages;
		dw->len = len;
		queue_work(decrypt_wq, &dw->decrypt);
		*num_mids = 0; /* worker thread takes care of finding mid */
		return -1;
	}

non_offloaded_decrypt:
	rc = decrypt_raw_data(server, buf, server->vals->read_rsp_size,
			      pages, npages, len, false);
	if (rc)
		goto free_pages;

	*mid = smb2_find_mid(server, buf);
	if (*mid == NULL)
		cifs_dbg(FYI, "mid not found\n");
	else {
		cifs_dbg(FYI, "mid found\n");
		(*mid)->decrypted = true;
		rc = handle_read_data(server, *mid, buf,
				      server->vals->read_rsp_size,
				      pages, npages, len, false);
		if (rc >= 0) {
			if (server->ops->is_network_name_deleted) {
				server->ops->is_network_name_deleted(buf,
								server);
			}
		}
	}

free_pages:
	for (i = i - 1; i >= 0; i--)
		put_page(pages[i]);
	kfree(pages);
	return rc;
discard_data:
	cifs_discard_remaining_data(server);
	goto free_pages;
}