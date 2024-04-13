GF_Err gf_filter_get_stats(GF_Filter *f, GF_FilterStats *stats)
{
	u32 i;
	Bool set_name=GF_FALSE;
	if (!stats || !f) return GF_BAD_PARAM;
	memset(stats, 0, sizeof(GF_FilterStats));
	stats->filter = f;
	stats->filter_alias = f->multi_sink_target;
	if (f->multi_sink_target) return GF_OK;
	
	stats->percent = f->status_percent>10000 ? -1 : (s32) f->status_percent;
	stats->status = f->status_str;
	stats->nb_pck_processed = f->nb_pck_processed;
	stats->nb_bytes_processed = f->nb_bytes_processed;
	stats->time_process = f->time_process;
	stats->nb_hw_pck_sent = f->nb_hw_pck_sent;
	stats->nb_pck_sent = f->nb_pck_sent;
	stats->nb_bytes_sent = f->nb_bytes_sent;
	stats->nb_tasks_done = f->nb_tasks_done;
	stats->nb_errors = f->nb_errors;
	stats->name = f->name;
	stats->reg_name = f->freg->name;
	stats->filter_id = f->id;
	stats->done = f->removed || f->finalized;
	if (stats->name && !strcmp(stats->name, stats->reg_name)) {
		set_name=GF_TRUE;
	}
	stats->report_updated = f->report_updated;
	f->report_updated = GF_FALSE;


	if (!stats->nb_pid_out && stats->nb_pid_in) stats->type = GF_FS_STATS_FILTER_RAWOUT;
	else if (!stats->nb_pid_in && stats->nb_pid_out) stats->type = GF_FS_STATS_FILTER_RAWIN;

	stats->nb_pid_out = f->num_output_pids;
	for (i=0; i<f->num_output_pids; i++) {
		GF_FilterPid *pid = gf_list_get(f->output_pids, i);
		stats->nb_out_pck += pid->nb_pck_sent;
		if (pid->has_seen_eos) stats->in_eos = GF_TRUE;

		if (pid->last_ts_sent.num * stats->last_ts_sent.den >= stats->last_ts_sent.num * pid->last_ts_sent.den)
			stats->last_ts_sent = pid->last_ts_sent;

		if (f->num_output_pids!=1) continue;

		if (!stats->codecid)
			stats->codecid = pid->codecid;
		if (!stats->stream_type)
			stats->stream_type = pid->stream_type;

		//set name if PID name is not a default generated one
		if (set_name && strncmp(pid->name, "PID", 3)) {
			stats->name = pid->name;
			set_name = GF_FALSE;
		}
	}
	gf_mx_p(f->tasks_mx);
	stats->nb_pid_in = f->num_input_pids;
	for (i=0; i<f->num_input_pids; i++) {
		GF_FilterPidInst *pidi = gf_list_get(f->input_pids, i);
		stats->nb_in_pck += pidi->nb_processed;
		if (pidi->is_end_of_stream) stats->in_eos = GF_TRUE;

		if (pidi->is_decoder_input) stats->type = GF_FS_STATS_FILTER_DECODE;
		else if (pidi->is_encoder_input) stats->type = GF_FS_STATS_FILTER_ENCODE;

		if (pidi->pid->stream_type==GF_STREAM_FILE)
			stats->type = GF_FS_STATS_FILTER_DEMUX;

		if (pidi->last_ts_drop.num * stats->last_ts_drop.den >= stats->last_ts_drop.num * pidi->last_ts_drop.den)
			stats->last_ts_drop = pidi->last_ts_drop;

		if ((f->num_input_pids!=1) && f->num_output_pids)
			continue;

		if (!stats->codecid)
			stats->codecid = pidi->pid->codecid;
		if (!stats->stream_type)
			stats->stream_type = pidi->pid->stream_type;

		if (set_name) {
			stats->name = pidi->pid->name;
			set_name = GF_FALSE;
		}
	}
	gf_mx_v(f->tasks_mx);
	if (!stats->type && stats->codecid) {
		if (!stats->nb_pid_out) {
			stats->type = GF_FS_STATS_FILTER_MEDIA_SINK;
		} else if (!stats->nb_pid_in) {
			stats->type = GF_FS_STATS_FILTER_MEDIA_SOURCE;
		}
	}
	return GF_OK;
}