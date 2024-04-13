static char *mif_getline(jas_stream_t *stream, char *buf, int bufsize)
{
	int c;
	char *bufptr;
	assert(bufsize > 0);

	bufptr = buf;
	while (bufsize > 1) {
		if ((c = mif_getc(stream)) == EOF) {
			break;
		}
		*bufptr++ = c;
		--bufsize;
		if (c == '\n') {
			break;
		}
	}
	*bufptr = '\0';
	if (!(bufptr = strchr(buf, '\n'))) {
		return 0;
	}
	*bufptr = '\0';
	return buf;
}
