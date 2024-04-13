static jpc_bitstream_t *jpc_bitstream_alloc()
{
	jpc_bitstream_t *bitstream;

	/* Allocate memory for the new bit stream object. */
	if (!(bitstream = jas_malloc(sizeof(jpc_bitstream_t)))) {
		return 0;
	}
	/* Initialize all of the data members. */
	bitstream->stream_ = 0;
	bitstream->cnt_ = 0;
	bitstream->flags_ = 0;
	bitstream->openmode_ = 0;

	return bitstream;
}
