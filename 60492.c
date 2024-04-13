chomp (char *s)
{
	size_t len;
	if (s && (len = strlen (s)) > 0 && s[len - 1] == '\n') {
		s[--len] = 0;
		if (len > 0 && s[len - 1] == '\r')
			s[--len] = 0;
	}
}
