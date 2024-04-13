static void jpg_finish_output(j_decompress_ptr cinfo, jpg_dest_t *dinfo)
{
	JAS_DBGLOG(10, ("jpg_finish_output(%p, %p)\n", cinfo, dinfo));

	/* Avoid compiler warnings about unused parameters. */
	cinfo = 0;
	dinfo = 0;
}
