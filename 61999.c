static void mif_hdr_destroy(mif_hdr_t *hdr)
{
	int cmptno;
	if (hdr->cmpts) {
		for (cmptno = 0; cmptno < hdr->numcmpts; ++cmptno) {
			mif_cmpt_destroy(hdr->cmpts[cmptno]);
		}
		jas_free(hdr->cmpts);
	}
	jas_free(hdr);
}
