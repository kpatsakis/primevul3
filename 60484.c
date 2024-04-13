strtoint(const char *const str)
{
	char *endptr;
	const int r = strtol(str, &endptr, 10);
	if (*endptr) return -1;
	return r;
}
