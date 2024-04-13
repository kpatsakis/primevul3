upnp_get_portmapping_number_of_entries()
{
	int n = 0, r = 0;
	unsigned short eport, iport;
	char protocol[4], iaddr[32], desc[64], rhost[32];
	unsigned int leaseduration;
	do {
		protocol[0] = '\0'; iaddr[0] = '\0'; desc[0] = '\0';
		r = upnp_get_redirection_infos_by_index(n, &eport, protocol, &iport,
		                                        iaddr, sizeof(iaddr),
		                                        desc, sizeof(desc),
		                                        rhost, sizeof(rhost),
		                                        &leaseduration);
		n++;
	} while(r==0);
	return (n-1);
}
