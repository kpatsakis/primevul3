int main(int argc, char * * argv)
{
	int ret = 0;
	int pid;
	struct sigaction sa;
	char buf[1500];
	ssize_t n;
	int s_ssdp = -1;	/* udp socket receiving ssdp packets */
#ifdef ENABLE_IPV6
	int s_ssdp6 = -1;	/* udp socket receiving ssdp packets IPv6*/
#else
#define s_ssdp6 (-1)
#endif
	int s_unix = -1;	/* unix socket communicating with clients */
	int s_ifacewatch = -1;	/* socket to receive Route / network interface config changes */
	int s;
	LIST_HEAD(reqstructhead, reqelem) reqlisthead;
	struct reqelem * req;
	struct reqelem * reqnext;
	fd_set readfds;
	const char * if_addr[MAX_IF_ADDR];
	int n_if_addr = 0;
	int i;
	const char * sockpath = "/var/run/minissdpd.sock";
	const char * pidfilename = "/var/run/minissdpd.pid";
	int debug_flag = 0;
	int ipv6 = 0;
	int deltadev = 0;
	struct sockaddr_in sendername;
	socklen_t sendername_len;
#ifdef ENABLE_IPV6
	struct sockaddr_in6 sendername6;
	socklen_t sendername6_len;
#endif

	LIST_INIT(&reqlisthead);
	LIST_INIT(&servicelisthead);
	/* process command line */
	for(i=1; i<argc; i++)
	{
		if(0==strcmp(argv[i], "-i")) {
			if(n_if_addr < MAX_IF_ADDR)
				if_addr[n_if_addr++] = argv[++i];
			else
				syslog(LOG_WARNING, "Max number of interface address set to %d, "
				       "ignoring %s", MAX_IF_ADDR, argv[++i]);
		} else if(0==strcmp(argv[i], "-d"))
			debug_flag = 1;
		else if(0==strcmp(argv[i], "-s"))
			sockpath = argv[++i];
		else if(0==strcmp(argv[i], "-p"))
			pidfilename = argv[++i];
#ifdef ENABLE_IPV6
		else if(0==strcmp(argv[i], "-6"))
			ipv6 = 1;
#endif
	}
	if(n_if_addr < 1)
	{
		fprintf(stderr,
		        "Usage: %s [-d] [-6] [-s socket] [-p pidfile] "
				"-i <interface> [-i <interface2>] ...\n",
		        argv[0]);
		fprintf(stderr,
		        "\n  <interface> is either an IPv4 address such as 192.168.1.42, or an\ninterface name such as eth0.\n");
		fprintf(stderr,
		        "\n  By default, socket will be open as %s\n"
		        "and pid written to file %s\n",
		        sockpath, pidfilename);
		return 1;
	}

	/* open log */
	openlog("minissdpd",
	        LOG_CONS|LOG_PID|(debug_flag?LOG_PERROR:0),
			LOG_MINISSDPD);
	if(!debug_flag) /* speed things up and ignore LOG_INFO and LOG_DEBUG */
		setlogmask(LOG_UPTO(LOG_NOTICE));

	if(checkforrunning(pidfilename) < 0)
	{
		syslog(LOG_ERR, "MiniSSDPd is already running. EXITING");
		return 1;
	}

	/* set signal handlers */
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sigterm;
	if(sigaction(SIGTERM, &sa, NULL))
	{
		syslog(LOG_ERR, "Failed to set SIGTERM handler. EXITING");
		ret = 1;
		goto quit;
	}
	if(sigaction(SIGINT, &sa, NULL))
	{
		syslog(LOG_ERR, "Failed to set SIGINT handler. EXITING");
		ret = 1;
		goto quit;
	}
	/* open route/interface config changes socket */
	s_ifacewatch = OpenAndConfInterfaceWatchSocket();
	/* open UDP socket(s) for receiving SSDP packets */
	s_ssdp = OpenAndConfSSDPReceiveSocket(n_if_addr, if_addr, 0);
	if(s_ssdp < 0)
	{
		syslog(LOG_ERR, "Cannot open socket for receiving SSDP messages, exiting");
		ret = 1;
		goto quit;
	}
#ifdef ENABLE_IPV6
	if(ipv6) {
		s_ssdp6 = OpenAndConfSSDPReceiveSocket(n_if_addr, if_addr, 1);
		if(s_ssdp6 < 0)
		{
			syslog(LOG_ERR, "Cannot open socket for receiving SSDP messages (IPv6), exiting");
			ret = 1;
			goto quit;
		}
	}
#endif
	/* Open Unix socket to communicate with other programs on
	 * the same machine */
	s_unix = OpenUnixSocket(sockpath);
	if(s_unix < 0)
	{
		syslog(LOG_ERR, "Cannot open unix socket for communicating with clients. Exiting");
		ret = 1;
		goto quit;
	}

	/* drop privileges */
#if 0
	/* if we drop privileges, how to unlink(/var/run/minissdpd.sock) ? */
	if(getuid() == 0) {
		struct passwd * user;
		struct group * group;
		user = getpwnam("nobody");
		if(!user) {
			syslog(LOG_ERR, "getpwnam(\"%s\") : %m", "nobody");
			ret = 1;
			goto quit;
		}
		group = getgrnam("nogroup");
		if(!group) {
			syslog(LOG_ERR, "getgrnam(\"%s\") : %m", "nogroup");
			ret = 1;
			goto quit;
		}
		if(setgid(group->gr_gid) < 0) {
			syslog(LOG_ERR, "setgit(%d) : %m", group->gr_gid);
			ret = 1;
			goto quit;
		}
		if(setuid(user->pw_uid) < 0) {
			syslog(LOG_ERR, "setuid(%d) : %m", user->pw_uid);
			ret = 1;
			goto quit;
		}
	}
#endif

	/* daemonize or in any case get pid ! */
	if(debug_flag)
		pid = getpid();
	else {
#ifdef USE_DAEMON
		if(daemon(0, 0) < 0)
			perror("daemon()");
		pid = getpid();
#else
		pid = daemonize();
#endif
	}

	writepidfile(pidfilename, pid);

	/* send M-SEARCH ssdp:all Requests */
	ssdpDiscoverAll(s_ssdp, 0);
	if(s_ssdp6 >= 0)
		ssdpDiscoverAll(s_ssdp6, 1);

	/* Main loop */
	while(!quitting)
	{
		/* fill readfds fd_set */
		FD_ZERO(&readfds);
		if(s_ssdp >= 0) {
			FD_SET(s_ssdp, &readfds);
		}
#ifdef ENABLE_IPV6
		if(s_ssdp6 >= 0) {
			FD_SET(s_ssdp6, &readfds);
		}
#endif
		if(s_ifacewatch >= 0) {
			FD_SET(s_ifacewatch, &readfds);
		}
		FD_SET(s_unix, &readfds);
		for(req = reqlisthead.lh_first; req; req = req->entries.le_next)
		{
			if(req->socket >= 0)
				FD_SET(req->socket, &readfds);
		}
		/* select call */
		if(select(FD_SETSIZE, &readfds, 0, 0, 0) < 0)
		{
			if(errno != EINTR)
			{
				syslog(LOG_ERR, "select: %m");
				break;	/* quit */
			}
			continue;	/* try again */
		}
#ifdef ENABLE_IPV6
		if((s_ssdp6 >= 0) && FD_ISSET(s_ssdp6, &readfds))
		{
			sendername6_len = sizeof(struct sockaddr_in6);
			n = recvfrom(s_ssdp6, buf, sizeof(buf), 0,
			             (struct sockaddr *)&sendername6, &sendername6_len);
			if(n<0)
			{
				 /* EAGAIN, EWOULDBLOCK, EINTR : silently ignore (try again next time)
				  * other errors : log to LOG_ERR */
				if(errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
					syslog(LOG_ERR, "recvfrom: %m");
			}
			else
			{
				/* Parse and process the packet received */
				/*printf("%.*s", n, buf);*/
				i = ParseSSDPPacket(s_ssdp6, buf, n,
				                    (struct sockaddr *)&sendername6);
				syslog(LOG_DEBUG, "** i=%d deltadev=%d **", i, deltadev);
				if(i==0 || (i*deltadev < 0))
				{
					if(deltadev > 0)
						syslog(LOG_NOTICE, "%d new devices added", deltadev);
					else if(deltadev < 0)
						syslog(LOG_NOTICE, "%d devices removed (good-bye!)", -deltadev);
					deltadev = i;
				}
				else if((i*deltadev) >= 0)
				{
					deltadev += i;
				}
			}
		}
#endif
		if(FD_ISSET(s_ssdp, &readfds))
		{
			sendername_len = sizeof(struct sockaddr_in);
			n = recvfrom(s_ssdp, buf, sizeof(buf), 0,
			             (struct sockaddr *)&sendername, &sendername_len);
			if(n<0)
			{
				 /* EAGAIN, EWOULDBLOCK, EINTR : silently ignore (try again next time)
				  * other errors : log to LOG_ERR */
				if(errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
					syslog(LOG_ERR, "recvfrom: %m");
			}
			else
			{
				/* Parse and process the packet received */
				/*printf("%.*s", n, buf);*/
				i = ParseSSDPPacket(s_ssdp, buf, n,
				                    (struct sockaddr *)&sendername);
				syslog(LOG_DEBUG, "** i=%d deltadev=%d **", i, deltadev);
				if(i==0 || (i*deltadev < 0))
				{
					if(deltadev > 0)
						syslog(LOG_NOTICE, "%d new devices added", deltadev);
					else if(deltadev < 0)
						syslog(LOG_NOTICE, "%d devices removed (good-bye!)", -deltadev);
					deltadev = i;
				}
				else if((i*deltadev) >= 0)
				{
					deltadev += i;
				}
			}
		}
		/* processing unix socket requests */
		for(req = reqlisthead.lh_first; req;)
		{
			reqnext = req->entries.le_next;
			if((req->socket >= 0) && FD_ISSET(req->socket, &readfds))
			{
				processRequest(req);
			}
			if(req->socket < 0)
			{
				LIST_REMOVE(req, entries);
				free(req);
			}
			req = reqnext;
		}
		/* processing new requests */
		if(FD_ISSET(s_unix, &readfds))
		{
			struct reqelem * tmp;
			s = accept(s_unix, NULL, NULL);
			if(s<0)
			{
				syslog(LOG_ERR, "accept(s_unix): %m");
			}
			else
			{
				syslog(LOG_INFO, "(s=%d) new request connection", s);
				if(!set_non_blocking(s))
					syslog(LOG_WARNING, "Failed to set new socket non blocking : %m");
				tmp = malloc(sizeof(struct reqelem));
				if(!tmp)
				{
					syslog(LOG_ERR, "cannot allocate memory for request");
					close(s);
				}
				else
				{
					tmp->socket = s;
					LIST_INSERT_HEAD(&reqlisthead, tmp, entries);
				}
			}
		}
		/* processing route/network interface config changes */
		if((s_ifacewatch >= 0) && FD_ISSET(s_ifacewatch, &readfds)) {
			ProcessInterfaceWatch(s_ifacewatch, s_ssdp, s_ssdp6, n_if_addr, if_addr);
		}
	}

	/* closing and cleaning everything */
quit:
	if(s_ssdp >= 0) {
		close(s_ssdp);
		s_ssdp = -1;
	}
#ifdef ENABLE_IPV6
	if(s_ssdp6 >= 0) {
		close(s_ssdp6);
		s_ssdp6 = -1;
	}
#endif
	if(s_unix >= 0) {
		close(s_unix);
		s_unix = -1;
		if(unlink(sockpath) < 0)
			syslog(LOG_ERR, "unlink(%s): %m", sockpath);
	}
	if(s_ifacewatch >= 0) {
		close(s_ifacewatch);
		s_ifacewatch = -1;
	}
	if(unlink(pidfilename) < 0)
		syslog(LOG_ERR, "unlink(%s): %m", pidfilename);
	closelog();
	return ret;
}
