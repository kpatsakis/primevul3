smb2_find_mid(struct TCP_Server_Info *server, char *buf)
{
	return __smb2_find_mid(server, buf, false);
}