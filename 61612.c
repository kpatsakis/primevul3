static bool _launch_job_test(uint32_t job_id)
{
	bool found = false;
	int j;

	slurm_mutex_lock(&job_state_mutex);
	for (j = 0; j < JOB_STATE_CNT; j++) {
		if (job_id == active_job_id[j]) {
			found = true;
			break;
		}
	}
	slurm_mutex_unlock(&job_state_mutex);
	return found;
}
