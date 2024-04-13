void gf_fs_print_stats(GF_FilterSession *fsess)
{
	u64 run_time=0, active_time=0, nb_tasks=0, nb_filters=0;
	u32 i, count;

	GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\n"));
	gf_mx_p(fsess->filters_mx);

	count=gf_list_count(fsess->filters);
	for (i=0; i<count; i++) {
		GF_Filter *f = gf_list_get(fsess->filters, i);
		if (f->multi_sink_target) continue;
		nb_filters++;
	}

	GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("Filter stats - %d filters\n", nb_filters));
	for (i=0; i<count; i++) {
		u32 k, ipids, opids;
		GF_Filter *f = gf_list_get(fsess->filters, i);
		if (f->multi_sink_target) continue;

		gf_mx_p(f->tasks_mx);

		ipids = f->num_input_pids;
		opids = f->num_output_pids;
		GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\tFilter "));
		print_filter_name(f, GF_FALSE, GF_FALSE);
		GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" : %d input pids %d output pids "LLU" tasks "LLU" us process time\n", ipids, opids, f->nb_tasks_done, f->time_process));

		if (ipids) {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\t\t"LLU" packets processed "LLU" bytes processed", f->nb_pck_processed, f->nb_bytes_processed));
			if (f->time_process) {
				GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" (%g pck/sec %g mbps)", (Double) f->nb_pck_processed*1000000/f->time_process, (Double) f->nb_bytes_processed*8/f->time_process));
			}
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\n"));
		}
		if (opids) {
			if (f->nb_hw_pck_sent) {
				GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\t\t"LLU" hardware frames sent", f->nb_hw_pck_sent));
				if (f->time_process) {
					GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" (%g pck/sec)", (Double) f->nb_hw_pck_sent*1000000/f->time_process));
				}

			} else if (f->nb_pck_sent) {
				GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\t\t"LLU" packets sent "LLU" bytes sent", f->nb_pck_sent, f->nb_bytes_sent));
				if (f->time_process) {
					GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" (%g pck/sec %g mbps)", (Double) f->nb_pck_sent*1000000/f->time_process, (Double) f->nb_bytes_sent*8/f->time_process));
				}
			}
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\n"));
		}

		for (k=0; k<ipids; k++) {
			GF_FilterPidInst *pid = gf_list_get(f->input_pids, k);
			if (!pid->pid) continue;
			if (pid->requires_full_data_block && (pid->nb_reagg_pck != pid->pid->nb_pck_sent) ) {
				GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\t\t* input PID %s: %d frames (%d packets) received\n", pid->pid->name, pid->nb_reagg_pck, pid->pid->nb_pck_sent));
			} else {
				GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\t\t* input PID %s: %d packets received\n", pid->pid->name, pid->pid->nb_pck_sent));
			}
		}
#ifndef GPAC_DISABLE_LOG
		for (k=0; k<opids; k++) {
			GF_FilterPid *pid = gf_list_get(f->output_pids, k);
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\t\t* output PID %s: %d packets sent\n", pid->name, pid->nb_pck_sent));
		}
		if (f->nb_errors) {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\t\t%d errors while processing\n", f->nb_errors));
		}
#endif

		gf_mx_v(f->tasks_mx);
	}
	gf_mx_v(fsess->filters_mx);

	count=gf_list_count(fsess->threads);
	GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("Session stats - threads %d\n", 1+count));

	GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\tThread %u: run_time "LLU" us active_time "LLU" us nb_tasks "LLU"\n", 1, fsess->main_th.run_time, fsess->main_th.active_time, fsess->main_th.nb_tasks));

	run_time+=fsess->main_th.run_time;
	active_time+=fsess->main_th.active_time;
	nb_tasks+=fsess->main_th.nb_tasks;

	for (i=0; i<count; i++) {
		GF_SessionThread *s = gf_list_get(fsess->threads, i);

		GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\tThread %u: run_time "LLU" us active_time "LLU" us nb_tasks "LLU"\n", i+2, s->run_time, s->active_time, s->nb_tasks));

		run_time+=s->run_time;
		active_time+=s->active_time;
		nb_tasks+=s->nb_tasks;
	}
	GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\nTotal: run_time "LLU" us active_time "LLU" us nb_tasks "LLU"\n", run_time, active_time, nb_tasks));
}