static int bmp_gobble(jas_stream_t *in, long n)
{
	while (--n >= 0) {
		if (jas_stream_getc(in) == EOF) {
			return -1;
		}
	}
	return 0;
}
