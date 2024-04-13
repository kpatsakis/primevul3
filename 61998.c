static mif_hdr_t *mif_hdr_create(int maxcmpts)
{
	mif_hdr_t *hdr;
	if (!(hdr = jas_malloc(sizeof(mif_hdr_t)))) {
		return 0;
	}
	hdr->numcmpts = 0;
	hdr->maxcmpts = 0;
	hdr->cmpts = 0;
	if (mif_hdr_growcmpts(hdr, maxcmpts)) {
		mif_hdr_destroy(hdr);
		return 0;
	}
	return hdr;
}
