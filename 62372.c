processMSEARCH(int s, const char * st, int st_len,
               const struct sockaddr * addr)
{
	struct service * serv;
#ifdef ENABLE_IPV6
	char buf[64];
#endif /* ENABLE_IPV6 */

	if(!st || st_len==0)
		return;
#ifdef ENABLE_IPV6
	sockaddr_to_string(addr, buf, sizeof(buf));
	syslog(LOG_INFO, "SSDP M-SEARCH from %s ST:%.*s",
	       buf, st_len, st);
#else	/* ENABLE_IPV6 */
	syslog(LOG_INFO, "SSDP M-SEARCH from %s:%d ST: %.*s",
	       inet_ntoa(((const struct sockaddr_in *)addr)->sin_addr),
	       ntohs(((const struct sockaddr_in *)addr)->sin_port),
	       st_len, st);
#endif	/* ENABLE_IPV6 */
	if(st_len==8 && (0==memcmp(st, "ssdp:all", 8))) {
		/* send a response for all services */
		for(serv = servicelisthead.lh_first;
		    serv;
		    serv = serv->entries.le_next) {
			SendSSDPMSEARCHResponse(s, addr,
			                        serv->st, serv->usn,
			                        serv->server, serv->location);
		}
	} else if(st_len > 5 && (0==memcmp(st, "uuid:", 5))) {
		/* find a matching UUID value */
		for(serv = servicelisthead.lh_first;
		    serv;
		    serv = serv->entries.le_next) {
			if(0 == strncmp(serv->usn, st, st_len)) {
				SendSSDPMSEARCHResponse(s, addr,
				                        serv->st, serv->usn,
				                        serv->server, serv->location);
			}
		}
	} else {
		/* find matching services */
		/* remove version at the end of the ST string */
		if(st[st_len-2]==':' && isdigit(st[st_len-1]))
			st_len -= 2;
		/* answer for each matching service */
		for(serv = servicelisthead.lh_first;
		    serv;
		    serv = serv->entries.le_next) {
			if(0 == strncmp(serv->st, st, st_len)) {
				SendSSDPMSEARCHResponse(s, addr,
				                        serv->st, serv->usn,
				                        serv->server, serv->location);
			}
		}
	}
}
