smb2_negotiate(const unsigned int xid,
	       struct cifs_ses *ses,
	       struct TCP_Server_Info *server)
{
	int rc;

	spin_lock(&GlobalMid_Lock);
	server->CurrentMid = 0;
	spin_unlock(&GlobalMid_Lock);
	rc = SMB2_negotiate(xid, ses, server);
	/* BB we probably don't need to retry with modern servers */
	if (rc == -EAGAIN)
		rc = -EHOSTDOWN;
	return rc;
}