static int_fast32_t pgx_getword(jas_stream_t *in, bool bigendian, int prec)
{
	uint_fast32_t val;
	int i;
	int j;
	int c;
	int wordsize;

	wordsize = (prec + 7) / 8;

	if (prec > 32) {
		goto error;
	}

	val = 0;
	for (i = 0; i < wordsize; ++i) {
		if ((c = jas_stream_getc(in)) == EOF) {
			goto error;
		}
		j = bigendian ? (wordsize - 1 - i) : i;
		val = val | ((c & 0xff) << (8 * j));
	}
	val &= (1 << prec) - 1;
	return val;

error:
	return -1;
}
