smb2_revert_current_mid(struct TCP_Server_Info *server, const unsigned int val)
{
	spin_lock(&GlobalMid_Lock);
	if (server->CurrentMid >= val)
		server->CurrentMid -= val;
	spin_unlock(&GlobalMid_Lock);
}