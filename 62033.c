int jpc_bitstream_fillbuf(jpc_bitstream_t *bitstream)
{
	int c;
	/* Note: The count has already been decremented by the caller. */
	assert(bitstream->openmode_ & JPC_BITSTREAM_READ);
	assert(bitstream->cnt_ <= 0);

	if (bitstream->flags_ & JPC_BITSTREAM_ERR) {
		bitstream->cnt_ = 0;
		return -1;
	}

	if (bitstream->flags_ & JPC_BITSTREAM_EOF) {
		bitstream->buf_ = 0x7f;
		bitstream->cnt_ = 7;
		return 1;
	}

	bitstream->buf_ = (bitstream->buf_ << 8) & 0xffff;
	if ((c = jas_stream_getc((bitstream)->stream_)) == EOF) {
		bitstream->flags_ |= JPC_BITSTREAM_EOF;
		return 1;
	}
	bitstream->cnt_ = (bitstream->buf_ == 0xff00) ? 6 : 7;
	bitstream->buf_ |= c & ((1 << (bitstream->cnt_ + 1)) - 1);
	return (bitstream->buf_ >> bitstream->cnt_) & 1;
}
