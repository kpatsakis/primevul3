static int createFromTiffTiles(TIFF *tif, gdImagePtr im, uint16 bps, uint16 photometric,
                               char has_alpha, char is_bw, int extra)
{
	uint16  planar;
	int im_width, im_height;
	int tile_width, tile_height;
	int  x, y, height, width;
	unsigned char *buffer;

	if (!TIFFGetField (tif, TIFFTAG_PLANARCONFIG, &planar)) {
		planar = PLANARCONFIG_CONTIG;
	}
	if (TIFFGetField (tif, TIFFTAG_IMAGEWIDTH, &im_width) == 0 ||
		TIFFGetField (tif, TIFFTAG_IMAGELENGTH, &im_height) == 0 ||
		TIFFGetField (tif, TIFFTAG_TILEWIDTH, &tile_width) ==  0 ||
		TIFFGetField (tif, TIFFTAG_TILELENGTH, &tile_height) == 0) {
		return FALSE;
	}

	buffer = (unsigned char *) gdMalloc (TIFFTileSize (tif));
	if (!buffer) {
		return FALSE;
	}

	for (y = 0; y < im_height; y += tile_height) {
		for (x = 0; x < im_width; x += tile_width) {
			TIFFReadTile(tif, buffer, x, y, 0, 0);
			width = MIN(im_width - x, tile_width);
			height = MIN(im_height - y, tile_height);
			if (bps == 16) {
			} else if (bps == 8) {
				readTiff8bit(buffer, im, photometric, x, y, width, height, has_alpha, extra, 0);
			} else if (is_bw) {
				readTiffBw(buffer, im, photometric, x, y, width, height, has_alpha, extra, 0);
			} else {
				/* TODO: implement some default reader or detect this case earlier use force_rgb */
			}
		}
	}
	gdFree(buffer);
	return TRUE;
}
