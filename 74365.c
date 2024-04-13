static ssize_t f_hid_opts_report_desc_store(struct config_item *item,
					    const char *page, size_t len)
{
	struct f_hid_opts *opts = to_f_hid_opts(item);
	int ret = -EBUSY;
	char *d;

	mutex_lock(&opts->lock);

	if (opts->refcnt)
		goto end;
	if (len > PAGE_SIZE) {
		ret = -ENOSPC;
		goto end;
	}
	d = kmemdup(page, len, GFP_KERNEL);
	if (!d) {
		ret = -ENOMEM;
		goto end;
	}
	kfree(opts->report_desc);
	opts->report_desc = d;
	opts->report_desc_length = len;
	opts->report_desc_alloc = true;
	ret = len;
end:
	mutex_unlock(&opts->lock);
	return ret;
}
