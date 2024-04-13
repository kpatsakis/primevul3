static jas_image_t *jpg_mkimage(j_decompress_ptr cinfo)
{
	jas_image_t *image;
	int cmptno;
	jas_image_cmptparm_t cmptparm;
	int numcmpts;

	JAS_DBGLOG(10, ("jpg_mkimage(%p)\n", cinfo));

	image = 0;
	numcmpts = cinfo->output_components;
	if (!(image = jas_image_create0())) {
		goto error;
	}
	for (cmptno = 0; cmptno < numcmpts; ++cmptno) {
		if (cinfo->image_width > JAS_IMAGE_COORD_MAX ||
		  cinfo->image_height > JAS_IMAGE_COORD_MAX) {
			goto error;
		}
		cmptparm.tlx = 0;
		cmptparm.tly = 0;
		cmptparm.hstep = 1;
		cmptparm.vstep = 1;
		cmptparm.width = cinfo->image_width;
		cmptparm.height = cinfo->image_height;
		cmptparm.prec = 8;
		cmptparm.sgnd = false;
		if (jas_image_addcmpt(image, cmptno, &cmptparm)) {
			goto error;
		}
	}

	if (numcmpts == 3) {
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

	return image;

error:
	if (image) {
		jas_image_destroy(image);
	}
	return 0;
}
