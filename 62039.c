jpc_bitstream_t *jpc_bitstream_sopen(jas_stream_t *stream, char *mode)
{
	jpc_bitstream_t *bitstream;

	/* Ensure that the open mode is valid. */
#if 0 /* This causes a string literal too long error (with c99 pedantic mode).  Why is this so? */
	assert(!strcmp(mode, "r") || !strcmp(mode, "w") || !strcmp(mode, "r+")
	  || !strcmp(mode, "w+"));
#endif

	if (!(bitstream = jpc_bitstream_alloc())) {
		return 0;
	}

	/* By default, do not close the underlying (character) stream, upon
	  the close of the bit stream. */
	bitstream->flags_ = JPC_BITSTREAM_NOCLOSE;

	bitstream->stream_ = stream;
	bitstream->openmode_ = (mode[0] == 'w') ? JPC_BITSTREAM_WRITE :
	  JPC_BITSTREAM_READ;

	/* Mark the data buffer as empty. */
	bitstream->cnt_ = (bitstream->openmode_ == JPC_BITSTREAM_READ) ? 0 : 8;
	bitstream->buf_ = 0;

	return bitstream;
}
