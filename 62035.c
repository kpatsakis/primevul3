int jpc_bitstream_inalign(jpc_bitstream_t *bitstream, int fillmask,
  int filldata)
{
	int n;
	int v;
	int u;
	int numfill;
	int m;

	numfill = 7;
	m = 0;
	v = 0;
	if (bitstream->cnt_ > 0) {
		n = bitstream->cnt_;
	} else if (!bitstream->cnt_) {
		n = ((bitstream->buf_ & 0xff) == 0xff) ? 7 : 0;
	} else {
		n = 0;
	}
	if (n > 0) {
		if ((u = jpc_bitstream_getbits(bitstream, n)) < 0) {
			return -1;
		}
		m += n;
		v = (v << n) | u;
	}
	if ((bitstream->buf_ & 0xff) == 0xff) {
		if ((u = jpc_bitstream_getbits(bitstream, 7)) < 0) {
			return -1;
		}
		v = (v << 7) | u;
		m += 7;
	}
	if (m > numfill) {
		v >>= m - numfill;
	} else {
		filldata >>= numfill - m;
		fillmask >>= numfill - m;
	}
	if (((~(v ^ filldata)) & fillmask) != fillmask) {
		/* The actual fill pattern does not match the expected one. */
		return 1;
	}

	return 0;
}
