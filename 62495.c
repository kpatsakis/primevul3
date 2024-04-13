static mif_hdr_t *mif_makehdrfromimage(jas_image_t *image)
{
	mif_hdr_t *hdr;
	int cmptno;
	mif_cmpt_t *cmpt;

	if (!(hdr = mif_hdr_create(jas_image_numcmpts(image)))) {
		return 0;
	}
	hdr->magic = MIF_MAGIC;
	hdr->numcmpts = jas_image_numcmpts(image);
	for (cmptno = 0; cmptno < hdr->numcmpts; ++cmptno) {
		hdr->cmpts[cmptno] = jas_malloc(sizeof(mif_cmpt_t));
		cmpt = hdr->cmpts[cmptno];
		cmpt->tlx = jas_image_cmpttlx(image, cmptno);
		cmpt->tly = jas_image_cmpttly(image, cmptno);
		cmpt->width = jas_image_cmptwidth(image, cmptno);
		cmpt->height = jas_image_cmptheight(image, cmptno);
		cmpt->sampperx = jas_image_cmpthstep(image, cmptno);
		cmpt->samppery = jas_image_cmptvstep(image, cmptno);
		cmpt->prec = jas_image_cmptprec(image, cmptno);
		cmpt->sgnd = jas_image_cmptsgnd(image, cmptno);
		cmpt->data = 0;
	}
	return hdr;
}
