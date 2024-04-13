int jpc_bitstream_needalign(jpc_bitstream_t *bitstream)
{
	if (bitstream->openmode_ & JPC_BITSTREAM_READ) {
		/* The bit stream is open for reading. */
		/* If there are any bits buffered for reading, or the
		  previous byte forced a stuffed bit, alignment is
		  required. */
		if ((bitstream->cnt_ < 8 && bitstream->cnt_ > 0) ||
		  ((bitstream->buf_ >> 8) & 0xff) == 0xff) {
			return 1;
		}
	} else if (bitstream->openmode_ & JPC_BITSTREAM_WRITE) {
		/* The bit stream is open for writing. */
		/* If there are any bits buffered for writing, or the
		  previous byte forced a stuffed bit, alignment is
		  required. */
		if ((bitstream->cnt_ < 8 && bitstream->cnt_ >= 0) ||
		  ((bitstream->buf_ >> 8) & 0xff) == 0xff) {
			return 1;
		}
	} else {
		/* This should not happen.  Famous last words, eh? :-) */
		assert(0);
		return -1;
	}
	return 0;
}
