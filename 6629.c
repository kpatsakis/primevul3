static void check_task_list(GF_FilterQueue *fq, GF_FSTask *task)
{
	if (fq) {
		gf_fq_enum(fq, check_task_list_enum, task);
	}
}