int mif_encode(jas_image_t *image, jas_stream_t *out, char *optstr)
{
	mif_hdr_t *hdr;
	jas_image_t *tmpimage;
	int fmt;
	int cmptno;
	mif_cmpt_t *cmpt;
	jas_image_cmptparm_t cmptparm;
	jas_seq2d_t *data;
	int_fast32_t x;
	int_fast32_t y;
	int bias;

	hdr = 0;
	tmpimage = 0;
	data = 0;

	if (optstr && *optstr != '\0') {
		jas_eprintf("warning: ignoring unsupported options\n");
	}

	if ((fmt = jas_image_strtofmt("pnm")) < 0) {
		jas_eprintf("error: PNM support required\n");
		goto error;
	}

	if (!(hdr = mif_makehdrfromimage(image))) {
		goto error;
	}
	if (mif_hdr_put(hdr, out)) {
		goto error;
	}

	/* Output component data. */
	for (cmptno = 0; cmptno < hdr->numcmpts; ++cmptno) {
		cmpt = hdr->cmpts[cmptno];
		if (!cmpt->data) {
			if (!(tmpimage = jas_image_create0())) {
				goto error;
			}	
			cmptparm.tlx = 0;
			cmptparm.tly = 0;
			cmptparm.hstep = cmpt->sampperx;
			cmptparm.vstep = cmpt->samppery;
			cmptparm.width = cmpt->width;
			cmptparm.height = cmpt->height;
			cmptparm.prec = cmpt->prec;
			cmptparm.sgnd = false;
			if (jas_image_addcmpt(tmpimage, jas_image_numcmpts(tmpimage),
			  &cmptparm)) {
				goto error;
			}
			jas_image_setclrspc(tmpimage, JAS_CLRSPC_SGRAY);
			jas_image_setcmpttype(tmpimage, 0,
			  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_GRAY_Y));
			if (!(data = jas_seq2d_create(0, 0, cmpt->width, cmpt->height))) {
				goto error;
			}
			if (jas_image_readcmpt(image, cmptno, 0, 0, cmpt->width,
			  cmpt->height, data)) {
				goto error;
			}
			if (cmpt->sgnd) {
				bias = 1 << (cmpt->prec - 1);
				for (y = 0; y < cmpt->height; ++y) {
					for (x = 0; x < cmpt->width; ++x) {
						*jas_seq2d_getref(data, x, y) += bias;
					}
				}
			}
			if (jas_image_writecmpt(tmpimage, 0, 0, 0, cmpt->width,
			  cmpt->height, data)) {
				goto error;
			}
			jas_seq2d_destroy(data);
			data = 0;
			if (jas_image_encode(tmpimage, out, fmt, 0)) {
				goto error;
			}
			jas_image_destroy(tmpimage);
			tmpimage = 0;
		}
	}

	mif_hdr_destroy(hdr);

	return 0;

error:
	if (hdr) {
		mif_hdr_destroy(hdr);
	}
	if (tmpimage) {
		jas_image_destroy(tmpimage);
	}
	if (data) {
		jas_seq2d_destroy(data);
	}
	return -1;
}
