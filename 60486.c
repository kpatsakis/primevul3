strtok_r(char *s, const char *delim, char **state) {
	char *cp, *start;
	start = cp = s ? s : *state;
	if (!cp)
		return NULL;
	while (*cp && !strchr(delim, *cp))
		++cp;
	if (!*cp) {
		if (cp == start)
			return NULL;
		*state = NULL;
		return start;
	} else {
		*cp++ = '\0';
		*state = cp;
		return start;
	}
}
