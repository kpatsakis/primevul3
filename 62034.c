int jpc_bitstream_getbit_func(jpc_bitstream_t *bitstream)
{
	int ret;
	JAS_DBGLOG(1000, ("jpc_bitstream_getbit_func(%p)\n", bitstream));
	ret = jpc_bitstream_getbit_macro(bitstream);
	JAS_DBGLOG(1000, ("jpc_bitstream_getbit_func -> %d\n", ret));
	return ret;
}
