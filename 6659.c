void gf_fs_run_step(GF_FilterSession *fsess)
{
	gf_fs_thread_proc(&fsess->main_th);
}