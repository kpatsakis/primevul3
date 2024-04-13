static int pgx_getuint32(jas_stream_t *in, uint_fast32_t *val)
{
	int c;
	uint_fast32_t v;

	do {
		if ((c = pgx_getc(in)) == EOF) {
			return -1;
		}
	} while (isspace(c));

	v = 0;
	while (isdigit(c)) {
		v = 10 * v + c - '0';
		if ((c = pgx_getc(in)) < 0) {
			return -1;
		}
	}
	if (!isspace(c)) {
		return -1;
	}
	*val = v;

	return 0;
}
