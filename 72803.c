process_colour_pointer_common(STREAM s, int bpp)
{
	extern RD_BOOL g_local_cursor;
	uint16 width, height, cache_idx, masklen, datalen;
	uint16 x, y;
	uint8 *mask;
	uint8 *data;
	RD_HCURSOR cursor;

	in_uint16_le(s, cache_idx);
	in_uint16_le(s, x);
	in_uint16_le(s, y);
	in_uint16_le(s, width);
	in_uint16_le(s, height);
	in_uint16_le(s, masklen);
	in_uint16_le(s, datalen);
	in_uint8p(s, data, datalen);
	in_uint8p(s, mask, masklen);

	logger(Protocol, Debug,
	       "process_colour_pointer_common(), new pointer %d with width %d and height %d",
	       cache_idx, width, height);

	/* keep hotspot within cursor bounding box */
	x = MIN(x, width - 1);
	y = MIN(y, height - 1);
	if (g_local_cursor)
		return;		/* don't bother creating a cursor we won't use */
	cursor = ui_create_cursor(x, y, width, height, mask, data, bpp);
	ui_set_cursor(cursor);
	cache_put_cursor(cache_idx, cursor);
}
