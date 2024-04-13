evdns_strtotimeval(const char *const str, struct timeval *out)
{
	double d;
	char *endptr;
	d = strtod(str, &endptr);
	if (*endptr) return -1;
	if (d < 0) return -1;
	out->tv_sec = (int) d;
	out->tv_usec = (int) ((d - (int) d)*1000000);
	if (out->tv_sec == 0 && out->tv_usec < 1000) /* less than 1 msec */
		return -1;
	return 0;
}
