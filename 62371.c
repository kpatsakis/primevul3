parselanaddr(struct lan_addr_s * lan_addr, const char * str)
{
	const char * p;
	int n;
	char tmp[16];

	memset(lan_addr, 0, sizeof(struct lan_addr_s));
	p = str;
	while(*p && *p != '/' && !isspace(*p))
		p++;
	n = p - str;
	if(!isdigit(str[0]) && n < (int)sizeof(lan_addr->ifname)) {
		/* not starting with a digit : suppose it is an interface name */
		memcpy(lan_addr->ifname, str, n);
		lan_addr->ifname[n] = '\0';
		if(getifaddr(lan_addr->ifname, lan_addr->str, sizeof(lan_addr->str),
		             &lan_addr->addr, &lan_addr->mask) < 0)
			goto parselan_error;
		/*printf("%s => %s\n", lan_addr->ifname, lan_addr->str);*/
	} else {
		if(n>15)
			goto parselan_error;
		memcpy(lan_addr->str, str, n);
		lan_addr->str[n] = '\0';
		if(!inet_aton(lan_addr->str, &lan_addr->addr))
			goto parselan_error;
	}
	if(*p == '/') {
		const char * q = ++p;
		while(*p && isdigit(*p))
			p++;
		if(*p=='.') {
			/* parse mask in /255.255.255.0 format */
			while(*p && (*p=='.' || isdigit(*p)))
				p++;
			n = p - q;
			if(n>15)
				goto parselan_error;
			memcpy(tmp, q, n);
			tmp[n] = '\0';
			if(!inet_aton(tmp, &lan_addr->mask))
				goto parselan_error;
		} else {
			/* it is a /24 format */
			int nbits = atoi(q);
			if(nbits > 32 || nbits < 0)
				goto parselan_error;
			lan_addr->mask.s_addr = htonl(nbits ? (0xffffffffu << (32 - nbits)) : 0);
		}
	} else if(lan_addr->mask.s_addr == 0) {
		/* by default, networks are /24 */
		lan_addr->mask.s_addr = htonl(0xffffff00u);
	}
#ifdef ENABLE_IPV6
	if(lan_addr->ifname[0] != '\0') {
		lan_addr->index = if_nametoindex(lan_addr->ifname);
		if(lan_addr->index == 0)
			fprintf(stderr, "Cannot get index for network interface %s",
			        lan_addr->ifname);
	} else {
		fprintf(stderr,
		        "Error: please specify LAN network interface by name instead of IPv4 address : %s\n",
		        str);
		return -1;
	}
#endif /* ENABLE_IPV6 */
	return 0;
parselan_error:
	fprintf(stderr, "Error parsing address/mask (or interface name) : %s\n",
	        str);
	return -1;
}
