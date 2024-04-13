static int mif_hdr_addcmpt(mif_hdr_t *hdr, int cmptno, mif_cmpt_t *cmpt)
{
	assert(cmptno >= hdr->numcmpts);
	if (hdr->numcmpts >= hdr->maxcmpts) {
		if (mif_hdr_growcmpts(hdr, hdr->numcmpts + 128)) {
			return -1;
		}
	}
	hdr->cmpts[hdr->numcmpts] = cmpt;
	++hdr->numcmpts;
	return 0;
}
