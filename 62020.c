jas_image_t *ras_decode(jas_stream_t *in, char *optstr)
{
	ras_hdr_t hdr;
	ras_cmap_t cmap;
	jas_image_t *image;
	jas_image_cmptparm_t cmptparms[3];
	jas_image_cmptparm_t *cmptparm;
	int clrspc;
	int numcmpts;
	int i;

	JAS_DBGLOG(10, ("ras_decode(%p, %p, \"%s\"\n", in, optstr ? optstr : ""));

	if (optstr) {
		jas_eprintf("warning: ignoring RAS decoder options\n");
	}

	/* Read the header. */
	if (ras_gethdr(in, &hdr)) {
		return 0;
	}

	/* Does the header information look reasonably sane? */
	if (hdr.magic != RAS_MAGIC || hdr.width <= 0 || hdr.height <= 0 ||
	  hdr.depth <= 0 || hdr.depth > 32) {
		return 0;
	}

	/* In the case of the old format, do not rely on the length field
	being properly specified.  Calculate the quantity from scratch. */
	if (hdr.type == RAS_TYPE_OLD) {
		hdr.length = RAS_ROWSIZE(&hdr) * hdr.height;
	}

	/* Calculate some quantities needed for creation of the image
	object. */
	if (RAS_ISRGB(&hdr)) {
		clrspc = JAS_CLRSPC_SRGB;
		numcmpts = 3;
	} else {
		clrspc = JAS_CLRSPC_SGRAY;
		numcmpts = 1;
	}
	for (i = 0, cmptparm = cmptparms; i < numcmpts; ++i, ++cmptparm) {
		cmptparm->tlx = 0;
		cmptparm->tly = 0;
		cmptparm->hstep = 1;
		cmptparm->vstep = 1;
		cmptparm->width = hdr.width;
		cmptparm->height = hdr.height;
		cmptparm->prec = RAS_ISRGB(&hdr) ? 8 : hdr.depth;
		cmptparm->sgnd = false;
	}
	/* Create the image object. */
	if (!(image = jas_image_create(numcmpts, cmptparms, JAS_CLRSPC_UNKNOWN))) {
		return 0;
	}

	/* Read the color map (if there is one). */
	if (ras_getcmap(in, &hdr, &cmap)) {
		jas_image_destroy(image);
		return 0;
	}

	/* Read the pixel data. */
	if (ras_getdata(in, &hdr, &cmap, image)) {
		jas_image_destroy(image);
		return 0;
	}

	jas_image_setclrspc(image, clrspc);
	if (clrspc == JAS_CLRSPC_SRGB) {
		jas_image_setcmpttype(image, 0,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_RGB_R));
		jas_image_setcmpttype(image, 1,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_RGB_G));
		jas_image_setcmpttype(image, 2,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_RGB_B));
	} else {
		jas_image_setcmpttype(image, 0,
		  JAS_IMAGE_CT_COLOR(JAS_CLRSPC_CHANIND_GRAY_Y));
	}

	return image;
}
