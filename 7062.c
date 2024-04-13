int strcasecmp(const char *s1, const char *s2)
{
	assert(s1);
	assert(s2);

	while (*s1) {
		int c1 = tolower(*(const unsigned char *)s1);
		int c2 = tolower(*(const unsigned char *)s2);

		if (c1 < c2)
			return -1;
		if (c1 > c2)
			return +1;

		++s1;
		++s2;
	}

	if (*s2 != 0)
		return -1;

	return 0;
}