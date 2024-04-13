void gf_fs_print_connections(GF_FilterSession *fsess)
{
	u32 i, count;
	Bool has_undefined=GF_FALSE;
	Bool has_connected=GF_FALSE;
	GF_List *filters_done;
	GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\n"));
	gf_mx_p(fsess->filters_mx);

	filters_done = gf_list_new();

	count=gf_list_count(fsess->filters);
	for (i=0; i<count; i++) {
		GF_Filter *f = gf_list_get(fsess->filters, i);
		//only dump sources
		if (f->num_input_pids) continue;
		if (!f->num_output_pids) continue;
		if (!has_connected) {
			has_connected = GF_TRUE;
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("Filters connected:\n"));
		}
		gf_fs_print_filter_outputs(f, filters_done, 0, NULL, NULL, 0, GF_FALSE);
	}

	gf_fs_print_not_connected_filters(fsess, filters_done, GF_FALSE);

	for (i=0; i<count; i++) {
		GF_Filter *f = gf_list_get(fsess->filters, i);
		if (f->multi_sink_target) continue;
		if (gf_list_find(filters_done, f)>=0) continue;
		if (!has_undefined) {
			has_undefined = GF_TRUE;
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("Filters in unknown connection state:\n"));
		}
		gf_fs_print_filter_outputs(f, filters_done, 0, NULL, NULL, 0, GF_FALSE);
	}

	gf_mx_v(fsess->filters_mx);
	gf_list_del(filters_done);
}