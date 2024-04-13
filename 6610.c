void gf_fs_check_graph_load(GF_FilterSession *fsess, Bool for_load)
{
	if (for_load) {
		if (!fsess->links || ! gf_list_count( fsess->links))
			gf_filter_sess_build_graph(fsess, NULL);
	} else {
		if (fsess->flags & GF_FS_FLAG_NO_GRAPH_CACHE)
			gf_filter_sess_reset_graph(fsess, NULL);
	}
}