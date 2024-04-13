smb2_set_related(struct smb_rqst *rqst)
{
	struct smb2_hdr *shdr;

	shdr = (struct smb2_hdr *)(rqst->rq_iov[0].iov_base);
	if (shdr == NULL) {
		cifs_dbg(FYI, "shdr NULL in smb2_set_related\n");
		return;
	}
	shdr->Flags |= SMB2_FLAGS_RELATED_OPERATIONS;
}