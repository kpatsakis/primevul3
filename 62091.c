pathcmp(const char *a, const char *b)
{
	/* Skip leading './' */
	if (a[0] == '.' && a[1] == '/' && a[2] != '\0')
		a += 2;
	if (b[0] == '.' && b[1] == '/' && b[2] != '\0')
		b += 2;
	/* Find the first difference, or return (0) if none. */
	while (*a == *b) {
		if (*a == '\0')
			return (0);
		a++;
		b++;
	}
	/*
	 * If one ends in '/' and the other one doesn't,
	 * they're the same.
	 */
	if (a[0] == '/' && a[1] == '\0' && b[0] == '\0')
		return (0);
	if (a[0] == '\0' && b[0] == '/' && b[1] == '\0')
		return (0);
	/* They're really different, return the correct sign. */
	return (*(const unsigned char *)a - *(const unsigned char *)b);
}
