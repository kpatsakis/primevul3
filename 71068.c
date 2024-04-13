processMSEARCH(int s, const char * st, size_t st_len,
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
	       buf, (int)st_len, st);
#else	/* ENABLE_IPV6 */
	syslog(LOG_INFO, "SSDP M-SEARCH from %s:%d ST: %.*s",
	       inet_ntoa(((const struct sockaddr_in *)addr)->sin_addr),
	       ntohs(((const struct sockaddr_in *)addr)->sin_port),
	       (int)st_len, st);
#endif	/* ENABLE_IPV6 */
	if(st_len==8 && (0==memcmp(st, "ssdp:all", 8))) {
		/* send a response for all services */
		for(serv = servicelisthead.lh_first;
		    serv;
		    serv = serv->entries.le_next) {
			SendSSDPMSEARCHResponse(s, addr,
			                        serv->st, strlen(serv->st), serv->usn,
			                        serv->server, serv->location);
		}
	} else if(st_len > 5 && (0==memcmp(st, "uuid:", 5))) {
		/* find a matching UUID value */
		for(serv = servicelisthead.lh_first;
		    serv;
		    serv = serv->entries.le_next) {
			if(0 == strncmp(serv->usn, st, st_len)) {
				SendSSDPMSEARCHResponse(s, addr,
				                        serv->st, strlen(serv->st), serv->usn,
				                        serv->server, serv->location);
			}
		}
	} else {
		size_t l;
		int st_ver = 0;
		char atoi_buffer[8];

		/* remove version at the end of the ST string */
		for (l = st_len; l > 0; l--) {
			if (st[l-1] == ':') {
				memset(atoi_buffer, 0, sizeof(atoi_buffer));
				memcpy(atoi_buffer, st + l, MIN((sizeof(atoi_buffer) - 1), st_len - l));
				st_ver = atoi(atoi_buffer);
				break;
			}
		}
		if (l == 0)
			l = st_len;
		/* answer for each matching service */
		/* From UPnP Device Architecture v1.1 :
		 * 1.3.2 [...] Updated versions of device and service types
		 * are REQUIRED to be full backward compatible with
		 * previous versions. Devices MUST respond to M-SEARCH
		 * requests for any supported version. For example, if a
		 * device implements “urn:schemas-upnporg:service:xyz:2”,
		 * it MUST respond to search requests for both that type
		 * and “urn:schemas-upnp-org:service:xyz:1”. The response
		 * MUST specify the same version as was contained in the
		 * search request. [...] */
		for(serv = servicelisthead.lh_first;
		    serv;
		    serv = serv->entries.le_next) {
			if(0 == strncmp(serv->st, st, l)) {
				syslog(LOG_DEBUG, "Found matching service : %s %s", serv->st, serv->location);
				SendSSDPMSEARCHResponse(s, addr,
				                        st, st_len, serv->usn,
				                        serv->server, serv->location);
			}
		}
	}
}
