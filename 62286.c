static void readTiffBw (const unsigned char *src,
			gdImagePtr im,
			uint16       photometric,
			int          startx,
			int          starty,
			int          width,
			int          height,
			char         has_alpha,
			int          extra,
			int          align)
{
	int x = startx, y = starty;

	(void)has_alpha;
	(void)extra;
	(void)align;

	for (y = starty; y < starty + height; y++) {
		for (x = startx; x < startx + width; x++) {
			register unsigned char curr = *src++;
			register unsigned char mask;

			if (photometric == PHOTOMETRIC_MINISWHITE) {
				curr = ~curr;
			}
			for (mask = 0x80; mask != 0 && x < startx + width; mask >>= 1) {
				gdImageSetPixel(im, x, y, ((curr & mask) != 0)?0:1);
			}
		}
	}
}
