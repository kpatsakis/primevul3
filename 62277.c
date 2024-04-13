BGD_DECLARE(gdImagePtr) gdImageCreateFromTiffCtx(gdIOCtx *infile)
{
	TIFF *tif;
	tiff_handle *th;

	uint16 bps, spp, photometric;
	uint16 orientation;
	int width, height;
	uint16 extra, *extra_types;
	uint16 planar;
	char	has_alpha, is_bw, is_gray;
	char	force_rgba = FALSE;
	char	save_transparent;
	int		image_type;
	int   ret;
	float res_float;

	gdImagePtr im = NULL;

	th = new_tiff_handle(infile);
	if (!th) {
		return NULL;
	}

	tif = TIFFClientOpen("", "rb", th, tiff_readproc,
	                     tiff_writeproc,
	                     tiff_seekproc,
	                     tiff_closeproc,
	                     tiff_sizeproc,
	                     tiff_mapproc,
	                     tiff_unmapproc);

	if (!tif) {
		gd_error("Cannot open TIFF image");
		gdFree(th);
		return NULL;
	}

	if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width)) {
		gd_error("TIFF error, Cannot read image width");
		goto error;
	}

	if (!TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
		gd_error("TIFF error, Cannot read image width");
		goto error;
	}

	TIFFGetFieldDefaulted (tif, TIFFTAG_BITSPERSAMPLE, &bps);

	/* Unsupported bps, force to RGBA */
	if (1/*bps > 8 && bps != 16*/) {
		force_rgba = TRUE;
	}

	TIFFGetFieldDefaulted (tif, TIFFTAG_SAMPLESPERPIXEL, &spp);

	if (!TIFFGetField (tif, TIFFTAG_EXTRASAMPLES, &extra, &extra_types)) {
		extra = 0;
	}

	if (!TIFFGetField (tif, TIFFTAG_PHOTOMETRIC, &photometric)) {
		uint16 compression;
		if (TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression) &&
		        (compression == COMPRESSION_CCITTFAX3 ||
		         compression == COMPRESSION_CCITTFAX4 ||
		         compression == COMPRESSION_CCITTRLE ||
		         compression == COMPRESSION_CCITTRLEW)) {
			gd_error("Could not get photometric. "
			        "Image is CCITT compressed, assuming min-is-white");
			photometric = PHOTOMETRIC_MINISWHITE;
		} else {
			gd_error("Could not get photometric. "
			        "Assuming min-is-black");

			photometric = PHOTOMETRIC_MINISBLACK;
		}
	}
	save_transparent = FALSE;

	/* test if the extrasample represents an associated alpha channel... */
	if (extra > 0 && (extra_types[0] == EXTRASAMPLE_ASSOCALPHA)) {
		has_alpha = TRUE;
		save_transparent = FALSE;
		--extra;
	} else if (extra > 0 && (extra_types[0] == EXTRASAMPLE_UNASSALPHA)) {
		has_alpha = TRUE;
		save_transparent = TRUE;
		--extra;
	} else if (extra > 0 && (extra_types[0] == EXTRASAMPLE_UNSPECIFIED)) {
		/* assuming unassociated alpha if unspecified */
		gd_error("alpha channel type not defined, assuming alpha is not premultiplied");
		has_alpha = TRUE;
		save_transparent = TRUE;
		--extra;
	} else {
		has_alpha = FALSE;
	}

	if (photometric == PHOTOMETRIC_RGB && spp > 3 + extra) {
		has_alpha = TRUE;
		extra = spp - 4;
	} else if (photometric != PHOTOMETRIC_RGB && spp > 1 + extra) {
		has_alpha = TRUE;
		extra = spp - 2;
	}

	is_bw = FALSE;
	is_gray = FALSE;

	switch (photometric) {
	case PHOTOMETRIC_MINISBLACK:
	case PHOTOMETRIC_MINISWHITE:
		if (!has_alpha && bps == 1 && spp == 1) {
			image_type = GD_INDEXED;
			is_bw = TRUE;
		} else {
			image_type = GD_GRAY;
		}
		break;

	case PHOTOMETRIC_RGB:
		image_type = GD_RGB;
		break;

	case PHOTOMETRIC_PALETTE:
		image_type = GD_INDEXED;
		break;

	default:
		force_rgba = TRUE;
		break;
	}

	if (!TIFFGetField (tif, TIFFTAG_PLANARCONFIG, &planar)) {
		planar = PLANARCONFIG_CONTIG;
	}

	/* Force rgba if image plans are not contiguous */
	if (force_rgba || planar != PLANARCONFIG_CONTIG) {
		image_type = GD_RGB;
	}

	if (!force_rgba &&
	        (image_type == GD_PALETTE || image_type == GD_INDEXED || image_type == GD_GRAY)) {
		im = gdImageCreate(width, height);
		if (!im) goto error;
		readTiffColorMap(im, tif, is_bw, photometric);
	} else {
		im = gdImageCreateTrueColor(width, height);
		if (!im) goto error;
	}

#ifdef DEBUG
	printf("force rgba: %i\n", force_rgba);
	printf("has_alpha: %i\n", has_alpha);
	printf("save trans: %i\n", save_transparent);
	printf("is_bw: %i\n", is_bw);
	printf("is_gray: %i\n", is_gray);
	printf("type: %i\n", image_type);
#else
	(void)is_gray;
	(void)save_transparent;
#endif

	if (force_rgba) {
		ret = createFromTiffRgba(tif, im);
	} else if (TIFFIsTiled(tif)) {
		ret = createFromTiffTiles(tif, im, bps, photometric, has_alpha, is_bw, extra);
	} else {
		ret = createFromTiffLines(tif, im, bps, photometric, has_alpha, is_bw, extra);
	}

	if (!ret) {
		gdImageDestroy(im);
		im = NULL;
		goto error;
	}

	if (TIFFGetField(tif, TIFFTAG_XRESOLUTION, &res_float)) { 
		im->res_x = (unsigned int)res_float;  //truncate
	}
	if (TIFFGetField(tif, TIFFTAG_YRESOLUTION, &res_float)) { 
		im->res_y = (unsigned int)res_float;  //truncate
	}

	if (TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation)) {
		switch (orientation) {
		case ORIENTATION_TOPLEFT:
		case ORIENTATION_TOPRIGHT:
		case ORIENTATION_BOTRIGHT:
		case ORIENTATION_BOTLEFT:
			break;

		default:
			gd_error("Orientation %d not handled yet!", orientation);
			break;
		}
	}
error:
	TIFFClose(tif);
	gdFree(th);
	return im;
}
