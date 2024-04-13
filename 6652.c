void gf_fs_post_task(GF_FilterSession *fsess, gf_fs_task_callback task_fun, GF_Filter *filter, GF_FilterPid *pid, const char *log_name, void *udta)
{
	gf_fs_post_task_ex(fsess, task_fun, filter, pid, log_name, udta, GF_FALSE, GF_FALSE);
}