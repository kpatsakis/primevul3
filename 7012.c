static char *strndup(const char *s, size_t n)
{
	char *r;

	r = malloc(n + 1);
	if (!r)
		return NULL;

	strncpy(r, s, n);
	r[n] = 0;

	return r;
}