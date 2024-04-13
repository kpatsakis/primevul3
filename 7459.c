smb2_parse_lease_buf(void *buf, unsigned int *epoch, char *lease_key)
{
	struct create_lease *lc = (struct create_lease *)buf;

	*epoch = 0; /* not used */
	if (lc->lcontext.LeaseFlags & SMB2_LEASE_FLAG_BREAK_IN_PROGRESS_LE)
		return SMB2_OPLOCK_LEVEL_NOCHANGE;
	return le32_to_cpu(lc->lcontext.LeaseState);
}