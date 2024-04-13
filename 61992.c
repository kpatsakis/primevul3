static mif_cmpt_t *mif_cmpt_create()
{
	mif_cmpt_t *cmpt;
	if (!(cmpt = jas_malloc(sizeof(mif_cmpt_t)))) {
		return 0;
	}
	memset(cmpt, 0, sizeof(mif_cmpt_t));
	return cmpt;
}
