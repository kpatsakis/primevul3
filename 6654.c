static void gf_fs_print_filter_outputs(GF_Filter *f, GF_List *filters_done, u32 indent, GF_FilterPid *pid, GF_Filter *alias_for, u32 src_num_tiled_pids, Bool skip_print)
{
	u32 i=0;
	u32 num_tile_pids = 0;

	if (!skip_print) {
		while (i<indent) {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("-"));
			i++;
		}

		if (src_num_tiled_pids>1) {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("(tilePID[%d]) ", src_num_tiled_pids));
		}
		else if (pid) {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("(PID %s) ", pid->name));
		}

		print_filter_name(f, GF_TRUE, GF_FALSE);
		if (f->id) {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" (ID=%s)\n", f->id));
		} else {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" (ptr=%p)\n", f));
		}
	}

	if (filters_done && (gf_list_find(filters_done, f)>=0))
		return;

	if (filters_done)
		gf_list_add(filters_done, f);
	if (alias_for && !skip_print) {
		GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" (<=> "));
		print_filter_name(alias_for, GF_TRUE, GF_TRUE);
		if (alias_for->id) {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" ID=%s", alias_for->id));
		} else {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" ptr=%p", alias_for));
		}
		GF_LOG(GF_LOG_INFO, GF_LOG_APP, (")\n"));
	}

	for (i=0; i<f->num_output_pids; i++) {
		u32 j, k;
		Bool is_tiled = GF_FALSE;
		Bool skip_tiled = skip_print;

		GF_FilterPid *pidout = gf_list_get(f->output_pids, i);
		const GF_PropertyValue *p = gf_filter_pid_get_property(pidout, GF_PROP_PID_CODECID);
		if (p && (p->value.uint==GF_CODECID_HEVC_TILES)) {
			is_tiled = GF_TRUE;
			//only print the first tile pid
			if (num_tile_pids) {
				skip_tiled = GF_TRUE;
			} else {
				for (j=i; j<f->num_output_pids; j++) {
					GF_FilterPid *apid = gf_list_get(f->output_pids, j);
					const GF_PropertyValue *p = gf_filter_pid_get_property(apid, GF_PROP_PID_CODECID);
					if (p && (p->value.uint==GF_CODECID_HEVC_TILES)) {
						num_tile_pids++;
					}
				}
			}
		}

		for (j=0; j<pidout->num_destinations; j++) {
			GF_FilterPidInst *pidi = gf_list_get(pidout->destinations, j);
			GF_Filter *alias = NULL;
			for (k=0; k<gf_list_count(f->destination_filters); k++) {
				alias = gf_list_get(f->destination_filters, k);
				if (alias->multi_sink_target == pidi->filter)
					break;
				alias = NULL;
			}
			if (alias) {
				gf_fs_print_filter_outputs(alias, filters_done, indent+1, pidout, pidi->filter, is_tiled ? num_tile_pids : src_num_tiled_pids, skip_tiled);
			} else {
				gf_fs_print_filter_outputs(pidi->filter, filters_done, indent+1, pidout, NULL, is_tiled ? num_tile_pids : src_num_tiled_pids, skip_tiled);
			}
		}
	}
}