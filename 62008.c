static int pgx_getsgnd(jas_stream_t *in, bool *sgnd)
{
	int c;

	do {
		if ((c = pgx_getc(in)) == EOF) {
			return -1;
		}
	} while (isspace(c));

#if 0
	if (c == '+') {
		*sgnd = false;
	} else if (c == '-') {
		*sgnd = true;
	} else {
		*sgnd = false;
		if (jas_stream_ungetc(in, c)) {
			goto error;
		}
		return 0;
	}

	while ((c = pgx_getc(in)) != EOF && !isspace(c)) {
		;
	}
	if (c == EOF) {
		goto error;
	}
#else
	if (c == '+' || c == '-') {
		*sgnd = (c == '-');
		while ((c = pgx_getc(in)) != EOF && !isspace(c)) {
			;
		}
		if (c == EOF) {
			goto error;
		}
	} else {
		*sgnd = false;
		if (jas_stream_ungetc(in, c)) {
			goto error;
		}
	}
#endif

	return 0;

error:
	return -1;
}
