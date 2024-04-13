static int bzImage64_cleanup(void *loader_data)
{
	struct bzimage64_data *ldata = loader_data;

	if (!ldata)
		return 0;

	kfree(ldata->bootparams_buf);
	ldata->bootparams_buf = NULL;

	return 0;
}
