decrypt_raw_data(struct TCP_Server_Info *server, char *buf,
		 unsigned int buf_data_size, struct page **pages,
		 unsigned int npages, unsigned int page_data_size,
		 bool is_offloaded)
{
	struct kvec iov[2];
	struct smb_rqst rqst = {NULL};
	int rc;

	iov[0].iov_base = buf;
	iov[0].iov_len = sizeof(struct smb2_transform_hdr);
	iov[1].iov_base = buf + sizeof(struct smb2_transform_hdr);
	iov[1].iov_len = buf_data_size;

	rqst.rq_iov = iov;
	rqst.rq_nvec = 2;
	rqst.rq_pages = pages;
	rqst.rq_npages = npages;
	rqst.rq_pagesz = PAGE_SIZE;
	rqst.rq_tailsz = (page_data_size % PAGE_SIZE) ? : PAGE_SIZE;

	rc = crypt_message(server, 1, &rqst, 0);
	cifs_dbg(FYI, "Decrypt message returned %d\n", rc);

	if (rc)
		return rc;

	memmove(buf, iov[1].iov_base, buf_data_size);

	if (!is_offloaded)
		server->total_read = buf_data_size + page_data_size;

	return rc;
}