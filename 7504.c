smb3_init_transform_rq(struct TCP_Server_Info *server, int num_rqst,
		       struct smb_rqst *new_rq, struct smb_rqst *old_rq)
{
	struct page **pages;
	struct smb2_transform_hdr *tr_hdr = new_rq[0].rq_iov[0].iov_base;
	unsigned int npages;
	unsigned int orig_len = 0;
	int i, j;
	int rc = -ENOMEM;

	for (i = 1; i < num_rqst; i++) {
		npages = old_rq[i - 1].rq_npages;
		pages = kmalloc_array(npages, sizeof(struct page *),
				      GFP_KERNEL);
		if (!pages)
			goto err_free;

		new_rq[i].rq_pages = pages;
		new_rq[i].rq_npages = npages;
		new_rq[i].rq_offset = old_rq[i - 1].rq_offset;
		new_rq[i].rq_pagesz = old_rq[i - 1].rq_pagesz;
		new_rq[i].rq_tailsz = old_rq[i - 1].rq_tailsz;
		new_rq[i].rq_iov = old_rq[i - 1].rq_iov;
		new_rq[i].rq_nvec = old_rq[i - 1].rq_nvec;

		orig_len += smb_rqst_len(server, &old_rq[i - 1]);

		for (j = 0; j < npages; j++) {
			pages[j] = alloc_page(GFP_KERNEL|__GFP_HIGHMEM);
			if (!pages[j])
				goto err_free;
		}

		/* copy pages form the old */
		for (j = 0; j < npages; j++) {
			char *dst, *src;
			unsigned int offset, len;

			rqst_page_get_length(&new_rq[i], j, &len, &offset);

			dst = (char *) kmap(new_rq[i].rq_pages[j]) + offset;
			src = (char *) kmap(old_rq[i - 1].rq_pages[j]) + offset;

			memcpy(dst, src, len);
			kunmap(new_rq[i].rq_pages[j]);
			kunmap(old_rq[i - 1].rq_pages[j]);
		}
	}

	/* fill the 1st iov with a transform header */
	fill_transform_hdr(tr_hdr, orig_len, old_rq, server->cipher_type);

	rc = crypt_message(server, num_rqst, new_rq, 1);
	cifs_dbg(FYI, "Encrypt message returned %d\n", rc);
	if (rc)
		goto err_free;

	return rc;

err_free:
	smb3_free_compound_rqst(num_rqst - 1, &new_rq[1]);
	return rc;
}