upnp_redirect_internal(const char * rhost, unsigned short eport,
                       const char * iaddr, unsigned short iport,
                       int proto, const char * desc,
                       unsigned int timestamp)
{
	/*syslog(LOG_INFO, "redirecting port %hu to %s:%hu protocol %s for: %s",
		eport, iaddr, iport, protocol, desc);			*/
	if(disable_port_forwarding)
		return -1;
	if(add_redirect_rule2(ext_if_name, rhost, eport, iaddr, iport, proto,
	                      desc, timestamp) < 0) {
		return -1;
	}

#ifdef ENABLE_LEASEFILE
	lease_file_add( eport, iaddr, iport, proto, desc, timestamp);
#endif
/*	syslog(LOG_INFO, "creating pass rule to %s:%hu protocol %s for: %s",
		iaddr, iport, protocol, desc);*/
	if(add_filter_rule2(ext_if_name, rhost, iaddr, eport, iport, proto, desc) < 0) {
		/* clean up the redirect rule */
#if !defined(__linux__)
		delete_redirect_rule(ext_if_name, eport, proto);
#endif
		return -1;
	}
	if(timestamp > 0) {
		if(!nextruletoclean_timestamp || (timestamp < nextruletoclean_timestamp))
			nextruletoclean_timestamp = timestamp;
	}
#ifdef ENABLE_EVENTS
	/* the number of port mappings changed, we must
	 * inform the subscribers */
	upnp_event_var_change_notify(EWanIPC);
#endif
	return 0;
}
