static int hdr_sz(
	size_t *size,
	const unsigned char **delta,
	const unsigned char *end)
{
	const unsigned char *d = *delta;
	size_t r = 0;
	unsigned int c, shift = 0;

	do {
		if (d == end) {
			giterr_set(GITERR_INVALID, "truncated delta");
			return -1;
		}

		c = *d++;
		r |= (c & 0x7f) << shift;
		shift += 7;
	} while (c & 0x80);
	*delta = d;
	*size = r;
	return 0;
}
