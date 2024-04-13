static int pnm_getsint(jas_stream_t *in, int wordsize, int_fast32_t *val)
{
	uint_fast32_t tmpval;

	if (pnm_getuint(in, wordsize, &tmpval)) {
		return -1;
	}
	if ((tmpval & (1 << (wordsize - 1))) != 0) {
		jas_eprintf("PNM decoder does not fully support signed data\n");
		return -1;
	}
	if (val) {
		*val = tmpval;
	}

	return 0;
}
