void gf_fs_cleanup_filters(GF_FilterSession *fsess)
{
	assert(fsess->pid_connect_tasks_pending);
	safe_int_dec(&fsess->pid_connect_tasks_pending);
}