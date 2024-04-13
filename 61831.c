static int jas_icctagtabent_cmp(const void *src, const void *dst)
{
	jas_icctagtabent_t *srctagtabent = JAS_CAST(jas_icctagtabent_t *, src);
	jas_icctagtabent_t *dsttagtabent = JAS_CAST(jas_icctagtabent_t *, dst);
	if (srctagtabent->off > dsttagtabent->off) {
		return 1;
	} else if (srctagtabent->off < dsttagtabent->off) {
		return -1;
	}
	return 0;
}
