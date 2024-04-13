static int pnm_getdata(jas_stream_t *in, pnm_hdr_t *hdr, jas_image_t *image)
{
	int ret;
#if 0
	int numcmpts;
#endif
	int cmptno;
	int fmt;
	jas_matrix_t *data[3];
	int x;
	int y;
	int_fast64_t v;
	int depth;
	int type;
	int c;
	int n;

	ret = -1;

#if 0
	numcmpts = jas_image_numcmpts(image);
#endif
	fmt = pnm_fmt(hdr->magic);
	type = pnm_type(hdr->magic);
	assert(type != PNM_TYPE_INVALID);
	depth = pnm_maxvaltodepth(hdr->maxval);

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	for (cmptno = 0; cmptno < hdr->numcmpts; ++cmptno) {
		if (!(data[cmptno] = jas_matrix_create(1, hdr->width))) {
			goto done;
		}
	}

	for (y = 0; y < hdr->height; ++y) {
		if (type == PNM_TYPE_PBM) {
			if (fmt == PNM_FMT_BIN) {
				for (x = 0; x < hdr->width;) {
					if ((c = jas_stream_getc(in)) == EOF) {
						goto done;
					}
					n = 8;
					while (n > 0 && x < hdr->width) {
						jas_matrix_set(data[0], 0, x, 1 - ((c >> 7) & 1));
						c <<= 1;
						--n;
						++x;
					}
				}
			} else {
				for (x = 0; x < hdr->width; ++x) {
					int uv;
					if (pnm_getbitstr(in, &uv)) {
						goto done;
					}
					jas_matrix_set(data[0], 0, x, 1 - uv);
				}
			}
		} else {
			for (x = 0; x < hdr->width; ++x) {
				for (cmptno = 0; cmptno < hdr->numcmpts; ++cmptno) {
					if (fmt == PNM_FMT_BIN) {
						/* The sample data is in binary format. */
						if (hdr->sgnd) {
							/* The sample data is signed. */
							int_fast32_t sv;
							if (pnm_getsint(in, depth, &sv)) {
								if (!pnm_allowtrunc) {
									goto done;
								}
								jas_eprintf("bad sample data\n");
								sv = 0;
							}
							v = sv;
						} else {
							/* The sample data is unsigned. */
							uint_fast32_t uv;
							if (pnm_getuint(in, depth, &uv)) {
								if (!pnm_allowtrunc) {
									goto done;
								}
								jas_eprintf("bad sample data\n");
								uv = 0;
							}
							v = uv;
						}
					} else {
						/* The sample data is in text format. */
						if (hdr->sgnd) {
							/* The sample data is signed. */
							int_fast32_t sv;
							if (pnm_getsintstr(in, &sv)) {
								if (!pnm_allowtrunc) {
									goto done;
								}
								jas_eprintf("bad sample data\n");
								sv = 0;
							}
							v = sv;
						} else {
							/* The sample data is unsigned. */
							uint_fast32_t uv;
							if (pnm_getuintstr(in, &uv)) {
								if (!pnm_allowtrunc) {
									goto done;
								}
								jas_eprintf("bad sample data\n");
								uv = 0;
							}
							v = uv;
						}
					}
					jas_matrix_set(data[cmptno], 0, x, v);
				}
			}
		}
		for (cmptno = 0; cmptno < hdr->numcmpts; ++cmptno) {
			if (jas_image_writecmpt(image, cmptno, 0, y, hdr->width, 1,
			  data[cmptno])) {
				goto done;
			}
		}
	}

	ret = 0;

done:

	for (cmptno = 0; cmptno < hdr->numcmpts; ++cmptno) {
		if (data[cmptno]) {
			jas_matrix_destroy(data[cmptno]);
		}
	}

	return ret;
}
