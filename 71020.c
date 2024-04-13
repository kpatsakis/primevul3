upnp_get_redirection_infos_by_index(int index,
                                    unsigned short * eport, char * protocol,
                                    unsigned short * iport,
                                    char * iaddr, int iaddrlen,
                                    char * desc, int desclen,
                                    char * rhost, int rhostlen,
                                    unsigned int * leaseduration)
{
	/*char ifname[IFNAMSIZ];*/
	int proto = 0;
	unsigned int timestamp;
	time_t current_time;

	if(desc && (desclen > 0))
		desc[0] = '\0';
	if(rhost && (rhostlen > 0))
		rhost[0] = '\0';
	if(get_redirect_rule_by_index(index, 0/*ifname*/, eport, iaddr, iaddrlen,
	                              iport, &proto, desc, desclen,
	                              rhost, rhostlen, &timestamp,
	                              0, 0) < 0)
		return -1;
	else
	{
		current_time = upnp_time();
		*leaseduration = (timestamp > (unsigned int)current_time)
		                 ? (timestamp - current_time)
		                 : 0;
		if(proto == IPPROTO_TCP)
			memcpy(protocol, "TCP", 4);
#ifdef IPPROTO_UDPLITE
		else if(proto == IPPROTO_UDPLITE)
			memcpy(protocol, "UDPLITE", 8);
#endif /* IPPROTO_UDPLITE */
		else
			memcpy(protocol, "UDP", 4);
		return 0;
	}
}
