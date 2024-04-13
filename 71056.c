upnp_event_notify_connect(struct upnp_event_notify * obj)
{
	unsigned int i;
	const char * p;
	unsigned short port;
#ifdef ENABLE_IPV6
	struct sockaddr_storage addr;
	socklen_t addrlen;
#else
	struct sockaddr_in addr;
	socklen_t addrlen;
#endif

	if(!obj)
		return;
	memset(&addr, 0, sizeof(addr));
	i = 0;
	if(obj->sub == NULL) {
		obj->state = EError;
		return;
	}
	p = obj->sub->callback;
	p += 7;	/* http:// */
#ifdef ENABLE_IPV6
	if(*p == '[') {	/* ip v6 */
		obj->addrstr[i++] = '[';
		p++;
		obj->ipv6 = 1;
		while(*p != ']' && i < (sizeof(obj->addrstr)-1))
			obj->addrstr[i++] = *(p++);
		if(*p == ']')
			p++;
		if(i < (sizeof(obj->addrstr)-1))
			obj->addrstr[i++] = ']';
	} else {
#endif
		while(*p != '/' && *p != ':' && i < (sizeof(obj->addrstr)-1))
			obj->addrstr[i++] = *(p++);
#ifdef ENABLE_IPV6
	}
#endif
	obj->addrstr[i] = '\0';
	if(*p == ':') {
		obj->portstr[0] = *p;
		i = 1;
		p++;
		port = (unsigned short)atoi(p);
		while(*p != '/') {
			if(i<7) obj->portstr[i++] = *p;
			p++;
		}
		obj->portstr[i] = 0;
	} else {
		port = 80;
		obj->portstr[0] = '\0';
	}
	obj->path = p;
#ifdef ENABLE_IPV6
	if(obj->ipv6) {
		char addrstr_tmp[48];
		struct sockaddr_in6 * sa = (struct sockaddr_in6 *)&addr;
		sa->sin6_family = AF_INET6;
		i = (int)strlen(obj->addrstr);
		if(i > 2) {
			i -= 2;
			memcpy(addrstr_tmp, obj->addrstr + 1, i);
			addrstr_tmp[i] = '\0';
			inet_pton(AF_INET6, addrstr_tmp, &(sa->sin6_addr));
		}
		sa->sin6_port = htons(port);
		addrlen = sizeof(struct sockaddr_in6);
	} else {
		struct sockaddr_in * sa = (struct sockaddr_in *)&addr;
		sa->sin_family = AF_INET;
		inet_pton(AF_INET, obj->addrstr, &(sa->sin_addr));
		sa->sin_port = htons(port);
		addrlen = sizeof(struct sockaddr_in);
	}
#else
	addr.sin_family = AF_INET;
	inet_aton(obj->addrstr, &addr.sin_addr);
	addr.sin_port = htons(port);
	addrlen = sizeof(struct sockaddr_in);
#endif
	syslog(LOG_DEBUG, "%s: '%s' %hu '%s'", "upnp_event_notify_connect",
	       obj->addrstr, port, obj->path);
	obj->state = EConnecting;
	if(connect(obj->s, (struct sockaddr *)&addr, addrlen) < 0) {
		if(errno != EINPROGRESS && errno != EWOULDBLOCK) {
			syslog(LOG_ERR, "%s: connect(%d, %s, %u): %m",
			       "upnp_event_notify_connect", obj->s,
			       obj->addrstr, addrlen);
			obj->state = EError;
		}
	}
}
