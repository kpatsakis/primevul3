smb3_parse_lease_buf(void *buf, unsigned int *epoch, char *lease_key)
{
	struct create_lease_v2 *lc = (struct create_lease_v2 *)buf;

	*epoch = le16_to_cpu(lc->lcontext.Epoch);
	if (lc->lcontext.LeaseFlags & SMB2_LEASE_FLAG_BREAK_IN_PROGRESS_LE)
		return SMB2_OPLOCK_LEVEL_NOCHANGE;
	if (lease_key)
		memcpy(lease_key, &lc->lcontext.LeaseKey, SMB2_LEASE_KEY_SIZE);
	return le32_to_cpu(lc->lcontext.LeaseState);
}