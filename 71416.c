int uverbs_dealloc_mw(struct ib_mw *mw)
{
	struct ib_pd *pd = mw->pd;
	int ret;

	ret = mw->device->ops.dealloc_mw(mw);
	if (!ret)
		atomic_dec(&pd->usecnt);
	return ret;
}
