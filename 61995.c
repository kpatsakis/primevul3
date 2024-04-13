static int mif_getc(jas_stream_t *in)
{
	int c;
	bool done;

	done = false;
	do {
		switch (c = jas_stream_getc(in)) {
		case EOF:
			done = true;
			break;
		case '#':
			for (;;) {
				if ((c = jas_stream_getc(in)) == EOF) {
					done = true;
					break;
				}	
				if (c == '\n') {
					done = true;
					break;
				}
			}
			break;
		case '\\':
			if (jas_stream_peekc(in) == '\n') {
				jas_stream_getc(in);
			}
			break;
		default:
			done = true;
			break;
		}
	} while (!done);

	return c;
}
