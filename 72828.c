rdp_out_ts_bitmap_capabilityset(STREAM s)
{
	logger(Protocol, Debug, "rdp_out_ts_bitmap_capabilityset(), %dx%d",
	       g_session_width, g_session_height);
	out_uint16_le(s, RDP_CAPSET_BITMAP);
	out_uint16_le(s, RDP_CAPLEN_BITMAP);
	out_uint16_le(s, g_server_depth);	/* preferredBitsPerPixel */
	out_uint16_le(s, 1);	/* receive1BitPerPixel (ignored, should be 1) */
	out_uint16_le(s, 1);	/* receive4BitPerPixel (ignored, should be 1) */
	out_uint16_le(s, 1);	/* receive8BitPerPixel (ignored, should be 1) */
	out_uint16_le(s, g_session_width);	/* desktopWidth */
	out_uint16_le(s, g_session_height);	/* desktopHeight */
	out_uint16_le(s, 0);	/* pad2Octets */
	out_uint16_le(s, 1);	/* desktopResizeFlag */
	out_uint16_le(s, 1);	/* bitmapCompressionFlag (must be 1) */
	out_uint8(s, 0);	/* highColorFlags (ignored, should be 0) */
	out_uint8(s, 0);	/* drawingFlags */
	out_uint16_le(s, 1);	/* multipleRectangleSupport (must be 1) */
	out_uint16_le(s, 0);	/* pad2OctetsB */
}
