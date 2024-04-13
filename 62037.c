int jpc_bitstream_outalign(jpc_bitstream_t *bitstream, int filldata)
{
	int n;
	int v;

	/* Ensure that this bit stream is open for writing. */
	assert(bitstream->openmode_ & JPC_BITSTREAM_WRITE);

	/* Ensure that the first bit of fill data is zero. */
	/* Note: The first bit of fill data must be zero.  If this were not
	  the case, the fill data itself could cause further bit stuffing to
	  be required (which would cause numerous complications). */
	assert(!(filldata & (~0x3f)));

	if (!bitstream->cnt_) {
		if ((bitstream->buf_ & 0xff) == 0xff) {
			n = 7;
			v = filldata;
		} else {
			n = 0;
			v = 0;
		}
	} else if (bitstream->cnt_ > 0 && bitstream->cnt_ < 8) {
		n = bitstream->cnt_;
		v = filldata >> (7 - n);
	} else {
		n = 0;
		v = 0;
		return 0;
	}

	/* Write the appropriate fill data to the bit stream. */
	if (n > 0) {
		if (jpc_bitstream_putbits(bitstream, n, v)) {
			return -1;
		}
	}
	if (bitstream->cnt_ < 8) {
		assert(bitstream->cnt_ >= 0 && bitstream->cnt_ < 8);
		assert((bitstream->buf_ & 0xff) != 0xff);
		/* Force the pending byte of output to be written to the
		  underlying (character) stream. */
		if (jas_stream_putc(bitstream->stream_, bitstream->buf_ & 0xff) == EOF) {
			return -1;
		}
		bitstream->cnt_ = 8;
		bitstream->buf_ = (bitstream->buf_ << 8) & 0xffff;
	}

	return 0;
}
