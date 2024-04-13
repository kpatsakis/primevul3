static ssize_t f_midi_opts_id_store(struct config_item *item,
				    const char *page, size_t len)
{
	struct f_midi_opts *opts = to_f_midi_opts(item);
	int ret;
	char *c;

	mutex_lock(&opts->lock);
	if (opts->refcnt) {
		ret = -EBUSY;
		goto end;
	}

	c = kstrndup(page, len, GFP_KERNEL);
	if (!c) {
		ret = -ENOMEM;
		goto end;
	}
	if (opts->id_allocated)
		kfree(opts->id);
	opts->id = c;
	opts->id_allocated = true;
	ret = len;
end:
	mutex_unlock(&opts->lock);
	return ret;
}
