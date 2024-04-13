upnp_delete_redirection(unsigned short eport, const char * protocol)
{
	syslog(LOG_INFO, "removing redirect rule port %hu %s", eport, protocol);
	return _upnp_delete_redir(eport, proto_atoi(protocol));
}
