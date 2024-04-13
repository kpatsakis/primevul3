GF_Err gf_fs_abort(GF_FilterSession *fsess, GF_FSFlushType flush_type)
{
	u32 i, count;
	Bool threaded;
	GF_LOG(GF_LOG_INFO, GF_LOG_FILTER, ("Session abort from user, stopping sources\n"));
	if (!fsess) return GF_BAD_PARAM;
	threaded = (!fsess->filters_mx && (fsess->main_th.th_id==gf_th_id())) ? GF_FALSE : GF_TRUE;

	if (flush_type==GF_FS_FLUSH_NONE) {
		fsess->in_final_flush = GF_TRUE;
		fsess->run_status = GF_EOS;
		return GF_OK;
	}

	fsess->in_final_flush = GF_TRUE;

	gf_mx_p(fsess->filters_mx);
	count = gf_list_count(fsess->filters);
	//disable all sources
	for (i=0; i<count; i++) {
		GF_Filter *filter = gf_list_get(fsess->filters, i);
		//force end of session on all sources, and on all filters connected to these sources, and dispatch end of stream on all outputs pids of these filters
		//if we don't propagate on connected filters, we take the risk of not deactivating demuxers working from file
		//(eg ignoring input packets)
		//
		//we shortcut the thread execution state here by directly calling set_eos, we need to lock/unlock our filters carefully
		//to avoid deadlocks or crashes
		gf_mx_p(filter->tasks_mx);

		if (!filter->num_input_pids) {
			u32 j, k, l;
			filter->disabled = GF_TRUE;
			for (j=0; j<filter->num_output_pids; j++) {
				const GF_PropertyValue *p;
				GF_FilterPid *pid = gf_list_get(filter->output_pids, j);
				//unlock before forcing eos as this could trigger a post task on a filter waiting for this mutex to be unlocked
				gf_mx_v(filter->tasks_mx);
				gf_filter_pid_set_eos(pid);
				gf_mx_p(filter->tasks_mx);
				//if the PID has a codecid set (demuxed pid, e.g. ffavin or other grabbers), do not force STOP on its destinations
				p = gf_filter_pid_get_property(pid, GF_PROP_PID_CODECID);
				if (p) continue;
				
				for (k=0; k<pid->num_destinations; k++) {
					Bool force_disable = GF_TRUE;
					GF_FilterPidInst *pidi = gf_list_get(pid->destinations, k);
					gf_mx_v(filter->tasks_mx);
					gf_mx_p(pidi->filter->tasks_mx);
					for (l=0; l<pidi->filter->num_output_pids; l++) {
						GF_FilterPid *opid = gf_list_get(pidi->filter->output_pids, l);
						//We cannot directly call process_event as this would make concurrent access to the filter
						//which we guarantee we will never do
						//but we don't want to send a regular stop event which will reset PID buffers, so:
						//- if called in main thread of session in single-thread mode we can safely force a STOP event
						//otherwise:
						//- post a task to the filter
						//- only disable the filter once the filter_abort has been called
						//- only move to EOS if no filter_abort is pending
						//
						if (opid->filter->freg->process_event) {
							if (threaded) {
								safe_int_inc(&opid->filter->abort_pending);
								gf_fs_post_task(opid->filter->session, filter_abort, opid->filter, opid, "filter_abort", NULL);
								force_disable = GF_FALSE;
							} else {
								GF_FilterEvent evt;
								GF_FEVT_INIT(evt, GF_FEVT_STOP, opid);

								opid->filter->freg->process_event(opid->filter, &evt);
								gf_filter_pid_set_eos(opid);
							}
						} else {
							gf_filter_pid_set_eos(opid);
						}
					}
					gf_mx_v(pidi->filter->tasks_mx);
					gf_mx_p(filter->tasks_mx);
					//no filter_abort pending, disable the filter
					if (force_disable)
						pidi->filter->disabled = GF_TRUE;
				}
			}
		}
		//fast flush and this is a sink: send a stop from all filters connected to the sink
		if ((flush_type==GF_FS_FLUSH_FAST) && !filter->num_output_pids) {
			u32 j;
			for (j=0; j<filter->num_input_pids; j++) {
				GF_FilterEvent evt;
				GF_FilterPidInst *pidi = gf_list_get(filter->input_pids, j);
				const GF_PropertyValue *p = gf_filter_pid_get_property((GF_FilterPid *) pidi, GF_PROP_PID_STREAM_TYPE);
				//if pid is of type FILE, we keep the last connections to the sink active so that muxers can still dispatch pending packets
				if (p && (p->value.uint==GF_STREAM_FILE)) {
					u32 k;
					gf_mx_p(pidi->pid->filter->tasks_mx);
					for (k=0; k<pidi->pid->filter->num_input_pids; k++) {
						GF_FilterPid *pid = gf_list_get(pidi->pid->filter->input_pids, k);
						GF_FEVT_INIT(evt, GF_FEVT_STOP, pid);
						gf_filter_pid_send_event(pid, &evt);
					}
					gf_mx_v(pidi->pid->filter->tasks_mx);
				}
				//otherwise send STOP right away
				else {
					GF_FEVT_INIT(evt, GF_FEVT_STOP, (GF_FilterPid *) pidi);
					gf_filter_pid_send_event((GF_FilterPid *) pidi, &evt);
				}
			}
		}

		gf_mx_v(filter->tasks_mx);
	}
	gf_mx_v(fsess->filters_mx);
	return GF_OK;
}