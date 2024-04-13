jas_image_t *pnm_decode(jas_stream_t *in, char *opts)
{
	pnm_hdr_t hdr;
	jas_image_t *image;
	jas_image_cmptparm_t cmptparms[3];
	jas_image_cmptparm_t *cmptparm;
	int i;

	JAS_DBGLOG(10, ("pnm_decode(%p, \"%s\")\n", in, opts ? opts : ""));

	if (opts) {
		jas_eprintf("warning: ignoring options\n");
	}

	/* Read the file header. */
	if (pnm_gethdr(in, &hdr)) {
		return 0;
	}
	JAS_DBGLOG(10, (
	  "magic %lx; width %lu; height %ld; numcmpts %d; maxval %ld; sgnd %d\n",
	  JAS_CAST(unsigned long, hdr.magic), JAS_CAST(long, hdr.width),
	  JAS_CAST(long, hdr.height), hdr.numcmpts, JAS_CAST(long, hdr.maxval),
	  hdr.sgnd)
	  );

	/* Create an image of the correct size. */
	for (i = 0, cmptparm = cmptparms; i < hdr.numcmpts; ++i, ++cmptparm) {
		cmptparm->tlx = 0;
		cmptparm->tly = 0;
		cmptparm->hstep = 1;
		cmptparm->vstep = 1;
		cmptparm->width = hdr.width;
		cmptparm->height = hdr.height;
		cmptparm->prec = pnm_maxvaltodepth(hdr.maxval);
		cmptparm->sgnd = hdr.sgnd;
	}
	if (!(image = jas_image_create(hdr.numcmpts, cmptparms,
	  JAS_CLRSPC_UNKNOWN))) {
		return 0;
	}

	if (hdr.numcmpts == 3) {
		jas_image_setclrspc(image, JAS_CLRSPC_SRGB);
		jas_image_setcmpttype(image, 0,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_RGB_R));
		jas_image_setcmpttype(image, 1,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_RGB_G));
		jas_image_setcmpttype(image, 2,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_RGB_B));
	} else {
		jas_image_setclrspc(image, JAS_CLRSPC_SGRAY);
		jas_image_setcmpttype(image, 0,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_GRAY_Y));
	}

	/* Read image data from stream into image. */
	if (pnm_getdata(in, &hdr, image)) {
		jas_image_destroy(image);
		return 0;
	}

	return image;
}
