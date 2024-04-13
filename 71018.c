upnp_get_portmappings_in_range(unsigned short startport,
                               unsigned short endport,
                               const char * protocol,
                               unsigned int * number)
{
	int proto;
	proto = proto_atoi(protocol);
	if(!number)
		return NULL;
	return get_portmappings_in_range(startport, endport, proto, number);
}
