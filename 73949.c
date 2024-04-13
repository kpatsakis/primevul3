dnnum_string(netdissect_options *ndo, u_short dnaddr)
{
	char *str;
	size_t siz;
	int area = (u_short)(dnaddr & AREAMASK) >> AREASHIFT;
	int node = dnaddr & NODEMASK;

	str = (char *)malloc(siz = sizeof("00.0000"));
	if (str == NULL)
		(*ndo->ndo_error)(ndo, "dnnum_string: malloc");
	snprintf(str, siz, "%d.%d", area, node);
	return(str);
}
