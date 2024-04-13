smb2_find_dequeue_mid(struct TCP_Server_Info *server, char *buf)
{
	return __smb2_find_mid(server, buf, true);
}