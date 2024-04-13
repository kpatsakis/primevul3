smb2_set_next_command(struct cifs_tcon *tcon, struct smb_rqst *rqst)
{
	struct smb2_hdr *shdr;
	struct cifs_ses *ses = tcon->ses;
	struct TCP_Server_Info *server = ses->server;
	unsigned long len = smb_rqst_len(server, rqst);
	int i, num_padding;

	shdr = (struct smb2_hdr *)(rqst->rq_iov[0].iov_base);
	if (shdr == NULL) {
		cifs_dbg(FYI, "shdr NULL in smb2_set_next_command\n");
		return;
	}

	/* SMB headers in a compound are 8 byte aligned. */

	/* No padding needed */
	if (!(len & 7))
		goto finished;

	num_padding = 8 - (len & 7);
	if (!smb3_encryption_required(tcon)) {
		/*
		 * If we do not have encryption then we can just add an extra
		 * iov for the padding.
		 */
		rqst->rq_iov[rqst->rq_nvec].iov_base = smb2_padding;
		rqst->rq_iov[rqst->rq_nvec].iov_len = num_padding;
		rqst->rq_nvec++;
		len += num_padding;
	} else {
		/*
		 * We can not add a small padding iov for the encryption case
		 * because the encryption framework can not handle the padding
		 * iovs.
		 * We have to flatten this into a single buffer and add
		 * the padding to it.
		 */
		for (i = 1; i < rqst->rq_nvec; i++) {
			memcpy(rqst->rq_iov[0].iov_base +
			       rqst->rq_iov[0].iov_len,
			       rqst->rq_iov[i].iov_base,
			       rqst->rq_iov[i].iov_len);
			rqst->rq_iov[0].iov_len += rqst->rq_iov[i].iov_len;
		}
		memset(rqst->rq_iov[0].iov_base + rqst->rq_iov[0].iov_len,
		       0, num_padding);
		rqst->rq_iov[0].iov_len += num_padding;
		len += num_padding;
		rqst->rq_nvec = 1;
	}

 finished:
	shdr->NextCommand = cpu_to_le32(len);
}