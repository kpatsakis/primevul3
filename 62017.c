static int pnm_getsintstr(jas_stream_t *in, int_fast32_t *val)
{
	int c;
	int s;
	int_fast32_t v;

	/* Discard any leading whitespace. */
	do {
		if ((c = pnm_getc(in)) == EOF) {
			return -1;
		}
	} while (isspace(c));

	/* Get the number, allowing for a negative sign. */
	s = 1;
	if (c == '-') {
		s = -1;
		if ((c = pnm_getc(in)) == EOF) {
			return -1;
		}
	} else if (c == '+') {
		if ((c = pnm_getc(in)) == EOF) {
			return -1;
		}
	}
	v = 0;
	while (isdigit(c)) {
		v = 10 * v + c - '0';
		if ((c = pnm_getc(in)) < 0) {
			return -1;
		}
	}

	/* The number must be followed by whitespace. */
	if (!isspace(c)) {
		return -1;
	}

	if (val) {
		*val = (s >= 0) ? v : (-v);
	}

	return 0;
}
