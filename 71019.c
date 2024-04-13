upnp_get_redirection_infos(unsigned short eport, const char * protocol,
                           unsigned short * iport,
                           char * iaddr, int iaddrlen,
                           char * desc, int desclen,
                           char * rhost, int rhostlen,
                           unsigned int * leaseduration)
{
	int r;
	unsigned int timestamp;
	time_t current_time;

	if(desc && (desclen > 0))
		desc[0] = '\0';
	if(rhost && (rhostlen > 0))
		rhost[0] = '\0';
	r = get_redirect_rule(ext_if_name, eport, proto_atoi(protocol),
	                      iaddr, iaddrlen, iport, desc, desclen,
	                      rhost, rhostlen, &timestamp,
	                      0, 0);
	if(r == 0 &&
	   timestamp > 0 &&
	   timestamp > (unsigned int)(current_time = upnp_time())) {
		*leaseduration = timestamp - current_time;
	} else {
		*leaseduration = 0;
	}
	return r;
}
