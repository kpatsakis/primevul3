static int ras_putint(jas_stream_t *out, int val)
{
	int x;
	int i;
	int c;

	x = val;
	for (i = 0; i < 4; i++) {
		c = (x >> 24) & 0xff;
		if (jas_stream_putc(out, c) == EOF) {
			return -1;
		}
		x <<= 8;
	}

	return 0;
}
