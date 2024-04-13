static ssize_t f_midi_opts_id_show(struct config_item *item, char *page)
{
	struct f_midi_opts *opts = to_f_midi_opts(item);
	int result;

	mutex_lock(&opts->lock);
	if (opts->id) {
		result = strlcpy(page, opts->id, PAGE_SIZE);
	} else {
		page[0] = 0;
		result = 0;
	}

	mutex_unlock(&opts->lock);

	return result;
}
