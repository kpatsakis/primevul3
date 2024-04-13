static void filter_abort(GF_FSTask *task)
{
	GF_FilterEvent evt;
	GF_FEVT_INIT(evt, GF_FEVT_STOP, task->pid);

	task->pid->filter->freg->process_event(task->pid->filter, &evt);
	gf_filter_pid_set_eos(task->pid);
	task->pid->filter->disabled = GF_TRUE;
	safe_int_dec(&task->pid->filter->abort_pending);

}