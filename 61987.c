static int jpg_copystreamtofile(FILE *out, jas_stream_t *in)
{
	int c;

	while ((c = jas_stream_getc(in)) != EOF) {
		if (fputc(c, out) == EOF) {
			return -1;
		}
	}
	if (jas_stream_error(in)) {
		return -1;
	}
	return 0;
}
