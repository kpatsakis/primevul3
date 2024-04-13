static void jpg_start_output(j_decompress_ptr cinfo, jpg_dest_t *dinfo)
{
	/* Avoid compiler warnings about unused parameters. */
	cinfo = 0;

	JAS_DBGLOG(10, ("jpg_start_output(%p, %p)\n", cinfo, dinfo));

	dinfo->row = 0;
}
