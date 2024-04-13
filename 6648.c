GF_Err gf_fs_post_user_task(GF_FilterSession *fsess, Bool (*task_execute) (GF_FilterSession *fsess, void *callback, u32 *reschedule_ms), void *udta_callback, const char *log_name)
{
	GF_UserTask *utask;
	char *_log_name;
	if (!fsess || !task_execute) return GF_BAD_PARAM;
	GF_SAFEALLOC(utask, GF_UserTask);
	if (!utask) return GF_OUT_OF_MEM;
	utask->fsess = fsess;
	utask->callback = udta_callback;
	utask->task_execute = task_execute;
	//dup mem for user task
	_log_name = gf_strdup(log_name ? log_name : "user_task");
	gf_fs_post_task(fsess, gf_fs_user_task, NULL, NULL, _log_name, utask);
	return GF_OK;
}