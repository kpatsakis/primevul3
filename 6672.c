GF_Err gf_filter_post_task(GF_Filter *filter, Bool (*task_execute) (GF_Filter *filter, void *callback, u32 *reschedule_ms), void *udta, const char *task_name)
{
	GF_UserTask *utask;
	if (!filter || !task_execute) return GF_BAD_PARAM;
	GF_SAFEALLOC(utask, GF_UserTask);
	if (!utask) return GF_OUT_OF_MEM;
	utask->callback = udta;
	utask->task_execute_filter = task_execute;
	utask->fsess = filter->session;
	gf_fs_post_task(filter->session, gf_fs_user_task, filter, NULL, task_name ? task_name : "user_task", utask);
	return GF_OK;
}