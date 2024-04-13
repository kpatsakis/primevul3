static int ras_getint(jas_stream_t *in, int_fast32_t *val)
{
	int x;
	int c;
	int i;

	x = 0;
	for (i = 0; i < 4; i++) {
		if ((c = jas_stream_getc(in)) == EOF) {
			return -1;
		}
		x = (x << 8) | (c & 0xff);
	}

	*val = x;
	return 0;
}
