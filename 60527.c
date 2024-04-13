evutil_parse_servname(const char *servname, const char *protocol,
    const struct evutil_addrinfo *hints)
{
	int n = parse_numeric_servname(servname);
	if (n>=0)
		return n;
#if defined(EVENT__HAVE_GETSERVBYNAME) || defined(_WIN32)
	if (!(hints->ai_flags & EVUTIL_AI_NUMERICSERV)) {
		struct servent *ent = getservbyname(servname, protocol);
		if (ent) {
			return ntohs(ent->s_port);
		}
	}
#endif
	return -1;
}
