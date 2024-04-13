static int bmp_getdata(jas_stream_t *in, bmp_info_t *info, jas_image_t *image)
{
	int i;
	int j;
	int y;
	jas_matrix_t *cmpts[3];
	int numpad;
	int red;
	int grn;
	int blu;
	int ret;
	int numcmpts;
	int cmptno;
	int ind;
	bmp_palent_t *palent;
	int mxind;
	int haspal;

	assert(info->depth == 8 || info->depth == 24);
	assert(info->enctype == BMP_ENC_RGB);

	numcmpts = bmp_numcmpts(info);
	haspal = bmp_haspal(info);

	ret = 0;
	for (i = 0; i < numcmpts; ++i) {
		cmpts[i] = 0;
	}

	/* Create temporary matrices to hold component data. */
	for (i = 0; i < numcmpts; ++i) {
		if (!(cmpts[i] = jas_matrix_create(1, info->width))) {
			ret = -1;
			goto bmp_getdata_done;
		}
	}

	/* Calculate number of padding bytes per row of image data. */
	numpad = (numcmpts * info->width) % 4;
	if (numpad) {
		numpad = 4 - numpad;
	}

	mxind = (1 << info->depth) - 1;
	for (i = 0; i < info->height; ++i) {
		for (j = 0; j < info->width; ++j) {
			if (haspal) {
				if ((ind = jas_stream_getc(in)) == EOF) {
					ret = -1;
					goto bmp_getdata_done;
				}
				if (ind > mxind) {
					ret = -1;
					goto bmp_getdata_done;
				}
				if (ind < info->numcolors) {
					palent = &info->palents[ind];
					red = palent->red;
					grn = palent->grn;
					blu = palent->blu;
				} else {
					red = ind;
					grn = ind;
					blu = ind;
				}
			} else {
				if ((blu = jas_stream_getc(in)) == EOF ||
				  (grn = jas_stream_getc(in)) == EOF ||
				  (red = jas_stream_getc(in)) == EOF) {
					ret = -1;
					goto bmp_getdata_done;
				}
			}
			if (numcmpts == 3) {
				jas_matrix_setv(cmpts[0], j, red);
				jas_matrix_setv(cmpts[1], j, grn);
				jas_matrix_setv(cmpts[2], j, blu);
			} else {
				jas_matrix_setv(cmpts[0], j, red);
			}
		}
		for (j = numpad; j > 0; --j) {
				if (jas_stream_getc(in) == EOF) {
					ret = -1;
					goto bmp_getdata_done;
				}
		}
		for (cmptno = 0; cmptno < numcmpts; ++cmptno) {
			y = info->topdown ? i : (info->height - 1 - i);
			if (jas_image_writecmpt(image, cmptno, 0, y, info->width,
			  1, cmpts[cmptno])) {
				ret = -1;
				goto bmp_getdata_done;
			}
		}
	}

bmp_getdata_done:
	/* Destroy the temporary matrices. */
	for (i = 0; i < numcmpts; ++i) {
		if (cmpts[i]) {
			jas_matrix_destroy(cmpts[i]);
		}
	}

	return ret;
}
