smb2_need_neg(struct TCP_Server_Info *server)
{
	return server->max_read == 0;
}