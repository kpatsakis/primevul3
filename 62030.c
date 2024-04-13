int jpc_bitstream_align(jpc_bitstream_t *bitstream)
{
	int ret;
	if (bitstream->openmode_ & JPC_BITSTREAM_READ) {
		ret = jpc_bitstream_inalign(bitstream, 0, 0);
	} else if (bitstream->openmode_ & JPC_BITSTREAM_WRITE) {
		ret = jpc_bitstream_outalign(bitstream, 0);
	} else {
		abort();
	}
	return ret;
}
