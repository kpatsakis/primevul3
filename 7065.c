static char *strdup(const char *str)
{
	size_t len;
	char *dup;

	len = strlen(str) + 1;
	dup = calloc(len, sizeof(char));
	if (!dup)
		return NULL;

	memcpy(dup, str, len);

	return dup;
}