Bool gf_fs_is_last_task(GF_FilterSession *fsess)
{
	if (!fsess) return GF_TRUE;
	if (fsess->tasks_pending>1) return GF_FALSE;
	if (gf_fq_count(fsess->main_thread_tasks)) return GF_FALSE;
	if (gf_fq_count(fsess->tasks)) return GF_FALSE;
	return GF_TRUE;
}