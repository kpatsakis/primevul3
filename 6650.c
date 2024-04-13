static void gf_fs_user_task(GF_FSTask *task)
{
	u32 reschedule_ms=0;
	GF_UserTask *utask = (GF_UserTask *)task->udta;
	task->schedule_next_time = 0;

#ifndef GPAC_DISABLE_REMOTERY
	gf_rmt_begin_hash(task->log_name, GF_RMT_AGGREGATE, &utask->rmt_hash);
#endif
	if (utask->task_execute) {
		task->requeue_request = utask->task_execute(utask->fsess, utask->callback, &reschedule_ms);
	} else if (task->filter) {
		task->requeue_request = utask->task_execute_filter(task->filter, utask->callback, &reschedule_ms);
	} else {
		task->requeue_request = 0;
	}
	gf_rmt_end();
	//if no requeue request or if we are in final flush, don't re-execute
	if (!task->requeue_request || utask->fsess->in_final_flush) {
		gf_free(utask);
		task->udta = NULL;
		//we duplicated the name for user tasks
		gf_free((char *) task->log_name);
		task->requeue_request = GF_FALSE;
	} else {
		task->schedule_next_time = gf_sys_clock_high_res() + 1000*reschedule_ms;
	}
}