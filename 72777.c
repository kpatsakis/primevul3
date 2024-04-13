process_brushcache(STREAM s, uint16 flags)
{
	UNUSED(flags);
	BRUSHDATA brush_data;
	uint8 cache_idx, colour_code, width, height, size, type;
	uint8 *comp_brush;
	int index;
	int Bpp;

	in_uint8(s, cache_idx);
	in_uint8(s, colour_code);
	in_uint8(s, width);
	in_uint8(s, height);
	in_uint8(s, type);	/* type, 0x8x = cached */
	in_uint8(s, size);

	logger(Graphics, Debug, "process_brushcache(), idx=%d, wd=%d, ht=%d, type=0x%x sz=%d",
	       cache_idx, width, height, type, size);

	if ((width == 8) && (height == 8))
	{
		if (colour_code == 1)
		{
			brush_data.colour_code = 1;
			brush_data.data_size = 8;
			brush_data.data = xmalloc(8);
			if (size == 8)
			{
				/* read it bottom up */
				for (index = 7; index >= 0; index--)
				{
					in_uint8(s, brush_data.data[index]);
				}
			}
			else
			{
				logger(Graphics, Warning,
				       "process_brushcache(), incompatible brush, colour_code %d size %d",
				       colour_code, size);
			}
			cache_put_brush_data(1, cache_idx, &brush_data);
		}
		else if ((colour_code >= 3) && (colour_code <= 6))
		{
			Bpp = colour_code - 2;
			brush_data.colour_code = colour_code;
			brush_data.data_size = 8 * 8 * Bpp;
			brush_data.data = xmalloc(8 * 8 * Bpp);
			if (size == 16 + 4 * Bpp)
			{
				in_uint8p(s, comp_brush, 16 + 4 * Bpp);
				process_compressed_8x8_brush_data(comp_brush, brush_data.data, Bpp);
			}
			else
			{
				in_uint8a(s, brush_data.data, 8 * 8 * Bpp);
			}
			cache_put_brush_data(colour_code, cache_idx, &brush_data);
		}
		else
		{
			logger(Graphics, Warning,
			       "process_brushcache(), incompatible brush, colour_code %d size %d",
			       colour_code, size);
		}
	}
	else
	{
		logger(Graphics, Warning,
		       "process_brushcache(), incompatible brush, width height %d %d", width,
		       height);
	}
}
