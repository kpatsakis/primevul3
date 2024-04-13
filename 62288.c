void tiffWriter(gdImagePtr image, gdIOCtx *out, int bitDepth)
{
	int x, y;
	int i;
	int r, g, b, a;
	TIFF *tiff;
	int width, height;
	int color;
	char *scan;
	int samplesPerPixel = 3;
	int bitsPerSample;
	int transparentColorR = -1;
	int transparentColorG = -1;
	int transparentColorB = -1;
	uint16 extraSamples[1];
	uint16 *colorMapRed = NULL;
	uint16 *colorMapGreen = NULL;
	uint16 *colorMapBlue = NULL;

	tiff_handle *th;

	th = new_tiff_handle(out);
	if (!th) {
		return;
	}
	extraSamples[0] = EXTRASAMPLE_ASSOCALPHA;

	/* read in the width/height of gd image */
	width = gdImageSX(image);
	height = gdImageSY(image);

	/* reset clip region to whole image */
	gdImageSetClip(image, 0, 0, width, height);

	/* handle old-style single-colour mapping to 100% transparency */
	if(image->transparent != -1) {
		/* set our 100% transparent colour value */
		transparentColorR = gdImageRed(image, image->transparent);
		transparentColorG = gdImageGreen(image, image->transparent);
		transparentColorB = gdImageBlue(image, image->transparent);
	}

	/* Open tiff file writing routines, but use special read/write/seek
	 * functions so that tiff lib writes correct bits of tiff content to
	 * correct areas of file opened and modifieable by the gdIOCtx functions
	 */
	tiff = TIFFClientOpen("", "w", th,	tiff_readproc,
			      tiff_writeproc,
			      tiff_seekproc,
			      tiff_closeproc,
			      tiff_sizeproc,
			      tiff_mapproc,
			      tiff_unmapproc);

	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC,
		     (bitDepth == 24) ? PHOTOMETRIC_RGB : PHOTOMETRIC_PALETTE);

	bitsPerSample = (bitDepth == 24 || bitDepth == 8) ? 8 : 1;
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, bitsPerSample);

	TIFFSetField(tiff, TIFFTAG_XRESOLUTION, (float)image->res_x);
	TIFFSetField(tiff, TIFFTAG_YRESOLUTION, (float)image->res_y);

	/* build the color map for 8 bit images */
	if(bitDepth != 24) {
		colorMapRed   = (uint16 *) gdMalloc(3 * (1 << bitsPerSample));
		if (!colorMapRed) {
			gdFree(th);
			return;
		}
		colorMapGreen = (uint16 *) gdMalloc(3 * (1 << bitsPerSample));
		if (!colorMapGreen) {
			gdFree(colorMapRed);
			gdFree(th);
			return;
		}
		colorMapBlue  = (uint16 *) gdMalloc(3 *  (1 << bitsPerSample));
		if (!colorMapBlue) {
			gdFree(colorMapRed);
			gdFree(colorMapGreen);
			gdFree(th);
			return;
		}

		for(i = 0; i < image->colorsTotal; i++) {
			colorMapRed[i]   = gdImageRed(image,i) + (gdImageRed(image,i) * 256);
			colorMapGreen[i] = gdImageGreen(image,i)+(gdImageGreen(image,i)*256);
			colorMapBlue[i]  = gdImageBlue(image,i) + (gdImageBlue(image,i)*256);
		}

		TIFFSetField(tiff, TIFFTAG_COLORMAP, colorMapRed, colorMapGreen,
			     colorMapBlue);
		samplesPerPixel = 1;
	}

	/* here, we check if the 'save alpha' flag is set on the source gd image */
	if ((bitDepth == 24) &&
	    (image->saveAlphaFlag || image->transparent != -1)) {
		/* so, we need to store the alpha values too!
		 * Also, tell TIFF what the extra sample means (associated alpha) */
		samplesPerPixel = 4;
		TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel);
		TIFFSetField(tiff, TIFFTAG_EXTRASAMPLES, 1, extraSamples);
	} else {
		TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel);
	}

	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, 1);

	if(overflow2(width, samplesPerPixel)) {
		if (colorMapRed)   gdFree(colorMapRed);
		if (colorMapGreen) gdFree(colorMapGreen);
		if (colorMapBlue)  gdFree(colorMapBlue);
		gdFree(th);
		return;
	}

	if(!(scan = (char *)gdMalloc(width * samplesPerPixel))) {
		if (colorMapRed)   gdFree(colorMapRed);
		if (colorMapGreen) gdFree(colorMapGreen);
		if (colorMapBlue)  gdFree(colorMapBlue);
		gdFree(th);
		return;
	}

	/* loop through y-coords, and x-coords */
	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			/* generate scan line for writing to tiff */
			color = gdImageGetPixel(image, x, y);

			a = (127 - gdImageAlpha(image, color)) * 2;
			a = (a == 0xfe) ? 0xff : a & 0xff;
			b = gdImageBlue(image, color);
			g = gdImageGreen(image, color);
			r = gdImageRed(image, color);

			/* if this pixel has the same RGB as the transparent colour,
			 * then set alpha fully transparent */
			if (transparentColorR == r &&
			    transparentColorG == g &&
			    transparentColorB == b) {
				a = 0x00;
			}

			if(bitDepth != 24) {
				/* write out 1 or 8 bit value in 1 byte
				 * (currently treats 1bit as 8bit) */
				scan[(x * samplesPerPixel) + 0] = color;
			} else {
				/* write out 24 bit value in 3 (or 4 if transparent) bytes */
				if(image->saveAlphaFlag || image->transparent != -1) {
					scan[(x * samplesPerPixel) + 3] = a;
				}

				scan[(x * samplesPerPixel) + 2] = b;
				scan[(x * samplesPerPixel) + 1] = g;
				scan[(x * samplesPerPixel) + 0] = r;
			}
		}

		/* Write the scan line to the tiff */
		if(TIFFWriteEncodedStrip(tiff, y, scan, width * samplesPerPixel) == -1) {
			if (colorMapRed)   gdFree(colorMapRed);
			if (colorMapGreen) gdFree(colorMapGreen);
			if (colorMapBlue)  gdFree(colorMapBlue);
			gdFree(th);
			/* error handler here */
			gd_error("Could not create TIFF\n");
			return;
		}
	}

	/* now cloase and free up resources */
	TIFFClose(tiff);
	gdFree(scan);
	gdFree(th);

	if(bitDepth != 24) {
		gdFree(colorMapRed);
		gdFree(colorMapGreen);
		gdFree(colorMapBlue);
	}
}
