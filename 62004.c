jas_image_t *pgx_decode(jas_stream_t *in, char *optstr)
{
	jas_image_t *image;
	pgx_hdr_t hdr;
	jas_image_cmptparm_t cmptparm;

	/* Avoid compiler warnings about unused parameters. */
	optstr = 0;

	image = 0;

	if (pgx_gethdr(in, &hdr)) {
		jas_eprintf("cannot get header\n");
		goto error;
	}

	if (jas_getdbglevel() >= 10) {
		pgx_dumphdr(stderr, &hdr);
	}

	if (!(image = jas_image_create0())) {
		goto error;
	}
	cmptparm.tlx = 0;
	cmptparm.tly = 0;
	cmptparm.hstep = 1;
	cmptparm.vstep = 1;
	cmptparm.width = hdr.width;
	cmptparm.height = hdr.height;
	cmptparm.prec = hdr.prec;
	cmptparm.sgnd = hdr.sgnd;
	if (jas_image_addcmpt(image, 0, &cmptparm)) {
		goto error;
	}
	if (pgx_getdata(in, &hdr, image)) {
		jas_eprintf("cannot get data\n");
		goto error;
	}

	jas_image_setclrspc(image, JAS_CLRSPC_SGRAY);
	jas_image_setcmpttype(image, 0,
	  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_GRAY_Y));

	return image;

error:
	if (image) {
		jas_image_destroy(image);
	}
	return 0;
}
