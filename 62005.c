static int pgx_getbyteorder(jas_stream_t *in, bool *bigendian)
{
	int c;
	char buf[2];

	do {
		if ((c = pgx_getc(in)) == EOF) {
			return -1;
		}
	} while (isspace(c));

	buf[0] = c;
	if ((c = pgx_getc(in)) == EOF) {
		goto error;
	}
	buf[1] = c;
	if (buf[0] == 'M' && buf[1] == 'L') {
		*bigendian = true;
	} else if (buf[0] == 'L' && buf[1] == 'M') {
		*bigendian = false;
	} else {
		goto error;
	}

	while ((c = pgx_getc(in)) != EOF && !isspace(c)) {
		;
	}
	if (c == EOF) {
		goto error;
	}
	return 0;

error:
	return -1;
}
