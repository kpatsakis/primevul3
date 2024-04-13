change_conf(struct TCP_Server_Info *server)
{
	server->credits += server->echo_credits + server->oplock_credits;
	server->oplock_credits = server->echo_credits = 0;
	switch (server->credits) {
	case 0:
		return 0;
	case 1:
		server->echoes = false;
		server->oplocks = false;
		break;
	case 2:
		server->echoes = true;
		server->oplocks = false;
		server->echo_credits = 1;
		break;
	default:
		server->echoes = true;
		if (enable_oplocks) {
			server->oplocks = true;
			server->oplock_credits = 1;
		} else
			server->oplocks = false;

		server->echo_credits = 1;
	}
	server->credits -= server->echo_credits + server->oplock_credits;
	return server->credits + server->echo_credits + server->oplock_credits;
}