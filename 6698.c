static void gf_fs_print_not_connected_filters(GF_FilterSession *fsess, GF_List *filters_done, Bool ignore_sinks)
{
	u32 i, count;
	Bool has_unconnected=GF_FALSE;
	count=gf_list_count(fsess->filters);
	for (i=0; i<count; i++) {
		GF_Filter *f = gf_list_get(fsess->filters, i);
		//only dump not connected ones
		if (f->num_input_pids || f->num_output_pids || f->multi_sink_target || f->nb_tasks_done) continue;
		if (ignore_sinks) {
			Bool has_outputs;
			if (f->forced_caps)
				has_outputs = gf_filter_has_out_caps(f->forced_caps, f->nb_forced_caps);
			else
				has_outputs = gf_filter_has_out_caps(f->freg->caps, f->freg->nb_caps);
			if (!has_outputs) continue;
		}

		if (!has_unconnected) {
			has_unconnected = GF_TRUE;
			GF_LOG(GF_LOG_WARNING, GF_LOG_APP, ("Filters not connected:\n"));
		}
		gf_fs_print_filter_outputs(f, filters_done, 0, NULL, NULL, 0, GF_FALSE);
	}
}