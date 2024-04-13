Bool gf_fs_fire_event(GF_FilterSession *fs, GF_Filter *f, GF_FilterEvent *evt, Bool upstream)
{
	Bool ret = GF_FALSE;
	if (!fs || !evt) return GF_FALSE;

	GF_FilterPid *on_pid = evt->base.on_pid;
	evt->base.on_pid = NULL;
	if (f) {
		if (evt->base.type==GF_FEVT_USER) {
			if (f->freg->process_event && f->event_target) {
				gf_mx_p(f->tasks_mx);
				f->freg->process_event(f, evt);
				gf_mx_v(f->tasks_mx);
				ret = GF_TRUE;
			}
		}
		if (!ret) {
			gf_mx_p(f->tasks_mx);
			if (f->num_output_pids && upstream) ret = GF_TRUE;
			else if (f->num_input_pids && !upstream) ret = GF_TRUE;
			gf_filter_send_event(f, evt, upstream);
			gf_mx_v(f->tasks_mx);
		}
	} else {
		u32 i, count;
		gf_fs_lock_filters(fs, GF_TRUE);
		count = gf_list_count(fs->filters);
		for (i=0; i<count; i++) {
			Bool canceled;
			f = gf_list_get(fs->filters, i);
			if (f->disabled || f->removed) continue;
			if (f->multi_sink_target) continue;
			if (!f->freg->process_event) continue;
			if (!f->event_target) continue;

			gf_mx_p(f->tasks_mx);
			canceled = f->freg->process_event(f, evt);
			gf_mx_v(f->tasks_mx);
			ret = GF_TRUE;
			if (canceled) break;
		}
		gf_fs_lock_filters(fs, GF_FALSE);
	}
	evt->base.on_pid = on_pid;
	return ret;
}