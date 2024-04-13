static int readTiffColorMap(gdImagePtr im, TIFF *tif, char is_bw, int photometric)
{
	uint16 *redcmap, *greencmap, *bluecmap;
	uint16 bps;
	int i;

	if (is_bw) {
		if (photometric == PHOTOMETRIC_MINISWHITE) {
			gdImageColorAllocate(im, 255,255,255);
			gdImageColorAllocate(im, 0, 0, 0);
		} else {
			gdImageColorAllocate(im, 0, 0, 0);
			gdImageColorAllocate(im, 255,255,255);
		}
	} else {
		uint16 min_sample_val, max_sample_val;

		if (!TIFFGetField(tif, TIFFTAG_MINSAMPLEVALUE, &min_sample_val)) {
			min_sample_val = 0;
		}
		if (!TIFFGetField(tif, TIFFTAG_MAXSAMPLEVALUE, &max_sample_val)) {
			max_sample_val = 255;
		}

		if (photometric == PHOTOMETRIC_MINISBLACK || photometric == PHOTOMETRIC_MINISWHITE) {
			/* TODO: use TIFFTAG_MINSAMPLEVALUE and TIFFTAG_MAXSAMPLEVALUE */
			/* Gray level palette */
			for (i=min_sample_val; i <= max_sample_val; i++) {
				gdImageColorAllocate(im, i,i,i);
			}
			return GD_SUCCESS;

		} else if (!TIFFGetField(tif, TIFFTAG_COLORMAP, &redcmap, &greencmap, &bluecmap)) {
			gd_error("Cannot read the color map");
			return GD_FAILURE;
		}

		TIFFGetFieldDefaulted(tif, TIFFTAG_BITSPERSAMPLE, &bps);

#define	CVT(x)		(((x) * 255) / ((1L<<16)-1))
		if (checkColorMap(1<<bps, redcmap, greencmap, bluecmap) == 16) {
			for (i = (1<<bps)-1; i > 0; i--) {
				redcmap[i] = CVT(redcmap[i]);
				greencmap[i] = CVT(greencmap[i]);
				bluecmap[i] = CVT(bluecmap[i]);
			}
		}
		for (i = 0; i < 256; i++) {
			gdImageColorAllocate(im, redcmap[i], greencmap[i], bluecmap[i]);
		}
#undef CVT
	}
	return GD_SUCCESS;
}
