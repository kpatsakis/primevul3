static int mif_hdr_growcmpts(mif_hdr_t *hdr, int maxcmpts)
{
	int cmptno;
	mif_cmpt_t **newcmpts;
	assert(maxcmpts >= hdr->numcmpts);
	newcmpts = (!hdr->cmpts) ? jas_alloc2(maxcmpts, sizeof(mif_cmpt_t *)) :
	  jas_realloc2(hdr->cmpts, maxcmpts, sizeof(mif_cmpt_t *));
	if (!newcmpts) {
		return -1;
	}
	hdr->maxcmpts = maxcmpts;
	hdr->cmpts = newcmpts;
	for (cmptno = hdr->numcmpts; cmptno < hdr->maxcmpts; ++cmptno) {
		hdr->cmpts[cmptno] = 0;
	}
	return 0;
}
