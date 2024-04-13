void gf_fs_add_filter_register(GF_FilterSession *fsess, const GF_FilterRegister *freg)
{
	if (!freg) return;

	if (!freg->name) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Filter missing name - ignoring\n"));
		return;
	}
	if (!freg->process) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Filter %s missing process function - ignoring\n", freg->name));
		return;
	}
	if (fsess->blacklist) {
		char *fname = strstr(fsess->blacklist, freg->name);
		if (fname) {
			u32 len = (u32) strlen(freg->name);
			if (!fname[len] || (fname[len] == fsess->sep_list)) {
				return;
			}
		}
	}
	gf_list_add(fsess->registry, (void *) freg);

	if (fsess->init_done && fsess->links && gf_list_count( fsess->links)) {
		gf_filter_sess_build_graph(fsess, freg);
	}
}