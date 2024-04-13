static GFINLINE void print_filter_name(GF_Filter *f, Bool skip_id, Bool skip_args)
{
	if (f->freg->flags & GF_FS_REG_SCRIPT) {
		GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" \"%s\"", f->name));
	} else {
		GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("%s", f->freg->name));
		if (strcmp(f->name, f->freg->name)) {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" \"%s\"", f->name));
		}
	}
	if (!skip_id && f->id) GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" ID %s", f->id));
	if (f->dynamic_filter || skip_args) return;

	if (!f->src_args && !f->orig_args && !f->dst_args && !f->dynamic_source_ids) return;
	GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" ("));
	if (f->src_args) {
		GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("%s", f->src_args));
	}
	else if (f->orig_args) {
		GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("%s", f->orig_args));
	}
	else if (f->dst_args) {
		GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("%s", f->dst_args));
	}

	if (f->dynamic_source_ids) GF_LOG(GF_LOG_INFO, GF_LOG_APP, (",resolved SID:%s", f->source_ids));
	GF_LOG(GF_LOG_INFO, GF_LOG_APP, (")"));
}