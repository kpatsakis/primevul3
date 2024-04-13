static void _launch_complete_rm(uint32_t job_id)
{
	int j;

	slurm_mutex_lock(&job_state_mutex);
	for (j = 0; j < JOB_STATE_CNT; j++) {
		if (job_id == active_job_id[j])
			break;
	}
	if (j < JOB_STATE_CNT && job_id == active_job_id[j]) {
		for (j = j + 1; j < JOB_STATE_CNT; j++) {
			active_job_id[j - 1] = active_job_id[j];
		}
		active_job_id[JOB_STATE_CNT - 1] = 0;
	}
	slurm_mutex_unlock(&job_state_mutex);
	_launch_complete_log("job remove", job_id);
}
