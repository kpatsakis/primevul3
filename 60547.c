test_for_getaddrinfo_hacks(void)
{
	int r, r2;
	struct evutil_addrinfo *ai=NULL, *ai2=NULL;
	struct evutil_addrinfo hints;

	memset(&hints,0,sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags =
#ifdef AI_NUMERICHOST
	    AI_NUMERICHOST |
#endif
#ifdef AI_NUMERICSERV
	    AI_NUMERICSERV |
#endif
	    0;
	r = getaddrinfo("1.2.3.4", "80", &hints, &ai);
	hints.ai_socktype = SOCK_STREAM;
	r2 = getaddrinfo("1.2.3.4", "80", &hints, &ai2);
	if (r2 == 0 && r != 0) {
		need_numeric_port_hack_=1;
	}
	if (ai2 && ai2->ai_protocol == 0) {
		need_socktype_protocol_hack_=1;
	}

	if (ai)
		freeaddrinfo(ai);
	if (ai2)
		freeaddrinfo(ai2);
	tested_for_getaddrinfo_hacks=1;
}
