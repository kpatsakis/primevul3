static void _launch_complete_log(char *type, uint32_t job_id)
{
#if 0
	int j;

	info("active %s %u", type, job_id);
	slurm_mutex_lock(&job_state_mutex);
	for (j = 0; j < JOB_STATE_CNT; j++) {
		if (active_job_id[j] != 0) {
			info("active_job_id[%d]=%u", j, active_job_id[j]);
		}
	}
	slurm_mutex_unlock(&job_state_mutex);
#endif
}
