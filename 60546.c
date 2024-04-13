parse_numeric_servname(const char *servname)
{
	int n;
	char *endptr=NULL;
	n = (int) strtol(servname, &endptr, 10);
	if (n>=0 && n <= 65535 && servname[0] && endptr && !endptr[0])
		return n;
	else
		return -1;
}
