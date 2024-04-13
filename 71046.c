PinholeVerification(struct upnphttp * h, char * int_ip, unsigned short int_port)
{
	int n;
	char senderAddr[INET6_ADDRSTRLEN]="";
	struct addrinfo hints, *ai, *p;
	struct in6_addr result_ip;

	/* Pinhole InternalClient address must correspond to the action sender */
	syslog(LOG_INFO, "Checking internal IP@ and port (Security policy purpose)");

	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;

	/* if ip not valid assume hostname and convert */
	if (inet_pton(AF_INET6, int_ip, &result_ip) <= 0)
	{
		n = getaddrinfo(int_ip, NULL, &hints, &ai);
		if(!n && ai->ai_family == AF_INET6)
		{
			for(p = ai; p; p = p->ai_next)
			{
				inet_ntop(AF_INET6, (struct in6_addr *) p, int_ip, sizeof(struct in6_addr));
				result_ip = *((struct in6_addr *) p);
				/* TODO : deal with more than one ip per hostname */
				break;
			}
		}
		else
		{
			syslog(LOG_ERR, "Failed to convert hostname '%s' to ip address", int_ip);
			SoapError(h, 402, "Invalid Args");
			return -1;
		}
        freeaddrinfo(p);
	}

	if(inet_ntop(AF_INET6, &(h->clientaddr_v6), senderAddr, INET6_ADDRSTRLEN) == NULL)
	{
		syslog(LOG_ERR, "inet_ntop: %m");
	}
#ifdef DEBUG
	printf("\tPinholeVerification:\n\t\tCompare sender @: %s\n\t\t  to intClient @: %s\n", senderAddr, int_ip);
#endif
	if(strcmp(senderAddr, int_ip) != 0)
	if(h->clientaddr_v6.s6_addr != result_ip.s6_addr)
	{
		syslog(LOG_INFO, "Client %s tried to access pinhole for internal %s and is not authorized to do it",
		       senderAddr, int_ip);
		SoapError(h, 606, "Action not authorized");
		return 0;
	}

	/* Pinhole InternalPort must be greater than or equal to 1024 */
	if (int_port < 1024)
	{
		syslog(LOG_INFO, "Client %s tried to access pinhole with port < 1024 and is not authorized to do it",
		       senderAddr);
		SoapError(h, 606, "Action not authorized");
		return 0;
	}
	return 1;
}
