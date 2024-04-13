static int verify_headers(const void *data, unsigned long size,
			  struct object *obj, struct fsck_options *options)
{
	const char *buffer = (const char *)data;
	unsigned long i;

	for (i = 0; i < size; i++) {
		switch (buffer[i]) {
		case '\0':
			return report(options, obj,
				FSCK_MSG_NUL_IN_HEADER,
				"unterminated header: NUL at offset %ld", i);
		case '\n':
			if (i + 1 < size && buffer[i + 1] == '\n')
				return 0;
		}
	}

	/*
	 * We did not find double-LF that separates the header
	 * and the body.  Not having a body is not a crime but
	 * we do want to see the terminating LF for the last header
	 * line.
	 */
	if (size && buffer[size - 1] == '\n')
		return 0;

	return report(options, obj,
		FSCK_MSG_UNTERMINATED_HEADER, "unterminated header");
}
