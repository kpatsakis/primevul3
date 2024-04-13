static void jp2_dec_destroy(jp2_dec_t *dec)
{
	if (dec->ihdr) {
		jp2_box_destroy(dec->ihdr);
	}
	if (dec->bpcc) {
		jp2_box_destroy(dec->bpcc);
	}
	if (dec->cdef) {
		jp2_box_destroy(dec->cdef);
	}
	if (dec->pclr) {
		jp2_box_destroy(dec->pclr);
	}
	if (dec->image) {
		jas_image_destroy(dec->image);
	}
	if (dec->cmap) {
		jp2_box_destroy(dec->cmap);
	}
	if (dec->colr) {
		jp2_box_destroy(dec->colr);
	}
	if (dec->chantocmptlut) {
		jas_free(dec->chantocmptlut);
	}
	jas_free(dec);
}
