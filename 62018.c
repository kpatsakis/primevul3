static int pnm_getuint(jas_stream_t *in, int wordsize, uint_fast32_t *val)
{
	uint_fast32_t tmpval;
	int c;
	int n;

	tmpval = 0;
	n = (wordsize + 7) / 8;
	while (--n >= 0) {
		if ((c = jas_stream_getc(in)) == EOF) {
			return -1;
		}
		tmpval = (tmpval << 8) | c;
	}
	tmpval &= (((uint_fast64_t) 1) << wordsize) - 1;
	if (val) {
		*val = tmpval;
	}

	return 0;
}
