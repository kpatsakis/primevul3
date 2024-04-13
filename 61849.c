static int getint(jas_stream_t *in, int sgnd, int prec, long *val)
{
	long v;
	int n;
	int c;
	assert((!sgnd && prec >= 1) || (sgnd && prec >= 2));
	n = (prec + 7) / 8;
	v = 0;
	while (--n >= 0) {
		if ((c = jas_stream_getc(in)) == EOF)
			return -1;
		v = (v << 8) | c;
	}
	v &= ((1 << prec) - 1);
	if (sgnd) {
		*val = decode_twos_comp(v, prec);
	} else {
		*val = v;
	}
	return 0;
}
