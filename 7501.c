smb2_can_echo(struct TCP_Server_Info *server)
{
	return server->echoes;
}