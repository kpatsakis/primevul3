smb2_is_session_expired(char *buf)
{
	struct smb2_hdr *shdr = (struct smb2_hdr *)buf;

	if (shdr->Status != STATUS_NETWORK_SESSION_EXPIRED &&
	    shdr->Status != STATUS_USER_SESSION_DELETED)
		return false;

	trace_smb3_ses_expired(le32_to_cpu(shdr->Id.SyncId.TreeId),
			       le64_to_cpu(shdr->SessionId),
			       le16_to_cpu(shdr->Command),
			       le64_to_cpu(shdr->MessageId));
	cifs_dbg(FYI, "Session expired or deleted\n");

	return true;
}