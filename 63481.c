size_t extract_content_length(char *buffer, size_t size)
{
	char *clen = strstr(buffer, CONTENT_LENGTH);
	size_t len;
	char *end;

	/* Pattern not found */
	if (!clen || clen > buffer + size)
		return SIZE_MAX;

	/* Content-Length extraction */
	len = strtoul(clen + strlen(CONTENT_LENGTH), &end, 10);
	if (*end)
		return SIZE_MAX;

	return len;
}
