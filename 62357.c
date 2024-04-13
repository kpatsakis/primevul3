OpenUnixSocket(const char * path)
{
	struct sockaddr_un addr;
	int s;
	int rv;
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	if(s < 0)
	{
		syslog(LOG_ERR, "socket(AF_UNIX): %m");
		return -1;
	}
	/* unlink the socket pseudo file before binding */
	rv = unlink(path);
	if(rv < 0 && errno != ENOENT)
	{
		syslog(LOG_ERR, "unlink(unixsocket, \"%s\"): %m", path);
		close(s);
		return -1;
	}
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path));
	if(bind(s, (struct sockaddr *)&addr,
	           sizeof(struct sockaddr_un)) < 0)
	{
		syslog(LOG_ERR, "bind(unixsocket, \"%s\"): %m", path);
		close(s);
		return -1;
	}
	else if(listen(s, 5) < 0)
	{
		syslog(LOG_ERR, "listen(unixsocket): %m");
		close(s);
		return -1;
	}
	/* Change rights so everyone can communicate with us */
	if(chmod(path, 0666) < 0)
	{
		syslog(LOG_WARNING, "chmod(\"%s\"): %m", path);
	}
	return s;
}
