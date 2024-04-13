write_ruleset_details(int s)
{
	int proto = 0;
	unsigned short eport, iport;
	char desc[64];
	char iaddr[32];
	char rhost[32];
	unsigned int timestamp;
	u_int64_t packets;
	u_int64_t bytes;
	int i = 0;
	char buffer[256];
	int n;

	write(s, "Ruleset :\n", 10);
	while(get_redirect_rule_by_index(i, 0/*ifname*/, &eport, iaddr, sizeof(iaddr),
	                                 &iport, &proto, desc, sizeof(desc),
	                                 rhost, sizeof(rhost),
	                                 &timestamp,
	                                 &packets, &bytes) >= 0)
	{
		n = snprintf(buffer, sizeof(buffer),
		             "%2d %s %s:%hu->%s:%hu "
		             "'%s' %u %" PRIu64 " %" PRIu64 "\n",
		             /*"'%s' %llu %llu\n",*/
		             i, proto_itoa(proto), rhost,
		             eport, iaddr, iport, desc, timestamp, packets, bytes);
		write(s, buffer, n);
		i++;
	}
}
