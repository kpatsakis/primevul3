handle_read_data(struct TCP_Server_Info *server, struct mid_q_entry *mid,
		 char *buf, unsigned int buf_len, struct page **pages,
		 unsigned int npages, unsigned int page_data_size,
		 bool is_offloaded)
{
	unsigned int data_offset;
	unsigned int data_len;
	unsigned int cur_off;
	unsigned int cur_page_idx;
	unsigned int pad_len;
	struct cifs_readdata *rdata = mid->callback_data;
	struct smb2_hdr *shdr = (struct smb2_hdr *)buf;
	struct bio_vec *bvec = NULL;
	struct iov_iter iter;
	struct kvec iov;
	int length;
	bool use_rdma_mr = false;

	if (shdr->Command != SMB2_READ) {
		cifs_server_dbg(VFS, "only big read responses are supported\n");
		return -ENOTSUPP;
	}

	if (server->ops->is_session_expired &&
	    server->ops->is_session_expired(buf)) {
		if (!is_offloaded)
			cifs_reconnect(server, true);
		return -1;
	}

	if (server->ops->is_status_pending &&
			server->ops->is_status_pending(buf, server))
		return -1;

	/* set up first two iov to get credits */
	rdata->iov[0].iov_base = buf;
	rdata->iov[0].iov_len = 0;
	rdata->iov[1].iov_base = buf;
	rdata->iov[1].iov_len =
		min_t(unsigned int, buf_len, server->vals->read_rsp_size);
	cifs_dbg(FYI, "0: iov_base=%p iov_len=%zu\n",
		 rdata->iov[0].iov_base, rdata->iov[0].iov_len);
	cifs_dbg(FYI, "1: iov_base=%p iov_len=%zu\n",
		 rdata->iov[1].iov_base, rdata->iov[1].iov_len);

	rdata->result = server->ops->map_error(buf, true);
	if (rdata->result != 0) {
		cifs_dbg(FYI, "%s: server returned error %d\n",
			 __func__, rdata->result);
		/* normal error on read response */
		if (is_offloaded)
			mid->mid_state = MID_RESPONSE_RECEIVED;
		else
			dequeue_mid(mid, false);
		return 0;
	}

	data_offset = server->ops->read_data_offset(buf);
#ifdef CONFIG_CIFS_SMB_DIRECT
	use_rdma_mr = rdata->mr;
#endif
	data_len = server->ops->read_data_length(buf, use_rdma_mr);

	if (data_offset < server->vals->read_rsp_size) {
		/*
		 * win2k8 sometimes sends an offset of 0 when the read
		 * is beyond the EOF. Treat it as if the data starts just after
		 * the header.
		 */
		cifs_dbg(FYI, "%s: data offset (%u) inside read response header\n",
			 __func__, data_offset);
		data_offset = server->vals->read_rsp_size;
	} else if (data_offset > MAX_CIFS_SMALL_BUFFER_SIZE) {
		/* data_offset is beyond the end of smallbuf */
		cifs_dbg(FYI, "%s: data offset (%u) beyond end of smallbuf\n",
			 __func__, data_offset);
		rdata->result = -EIO;
		if (is_offloaded)
			mid->mid_state = MID_RESPONSE_MALFORMED;
		else
			dequeue_mid(mid, rdata->result);
		return 0;
	}

	pad_len = data_offset - server->vals->read_rsp_size;

	if (buf_len <= data_offset) {
		/* read response payload is in pages */
		cur_page_idx = pad_len / PAGE_SIZE;
		cur_off = pad_len % PAGE_SIZE;

		if (cur_page_idx != 0) {
			/* data offset is beyond the 1st page of response */
			cifs_dbg(FYI, "%s: data offset (%u) beyond 1st page of response\n",
				 __func__, data_offset);
			rdata->result = -EIO;
			if (is_offloaded)
				mid->mid_state = MID_RESPONSE_MALFORMED;
			else
				dequeue_mid(mid, rdata->result);
			return 0;
		}

		if (data_len > page_data_size - pad_len) {
			/* data_len is corrupt -- discard frame */
			rdata->result = -EIO;
			if (is_offloaded)
				mid->mid_state = MID_RESPONSE_MALFORMED;
			else
				dequeue_mid(mid, rdata->result);
			return 0;
		}

		rdata->result = init_read_bvec(pages, npages, page_data_size,
					       cur_off, &bvec);
		if (rdata->result != 0) {
			if (is_offloaded)
				mid->mid_state = MID_RESPONSE_MALFORMED;
			else
				dequeue_mid(mid, rdata->result);
			return 0;
		}

		iov_iter_bvec(&iter, WRITE, bvec, npages, data_len);
	} else if (buf_len >= data_offset + data_len) {
		/* read response payload is in buf */
		WARN_ONCE(npages > 0, "read data can be either in buf or in pages");
		iov.iov_base = buf + data_offset;
		iov.iov_len = data_len;
		iov_iter_kvec(&iter, WRITE, &iov, 1, data_len);
	} else {
		/* read response payload cannot be in both buf and pages */
		WARN_ONCE(1, "buf can not contain only a part of read data");
		rdata->result = -EIO;
		if (is_offloaded)
			mid->mid_state = MID_RESPONSE_MALFORMED;
		else
			dequeue_mid(mid, rdata->result);
		return 0;
	}

	length = rdata->copy_into_pages(server, rdata, &iter);

	kfree(bvec);

	if (length < 0)
		return length;

	if (is_offloaded)
		mid->mid_state = MID_RESPONSE_RECEIVED;
	else
		dequeue_mid(mid, false);
	return length;
}