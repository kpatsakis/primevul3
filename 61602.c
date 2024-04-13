static bool _is_batch_job_finished(uint32_t job_id)
{
	bool found_job = false;
	int i;

	slurm_mutex_lock(&fini_mutex);
	for (i = 0; i < FINI_JOB_CNT; i++) {
		if (fini_job_id[i] == job_id) {
			found_job = true;
			break;
		}
	}
	slurm_mutex_unlock(&fini_mutex);

	return found_job;
}
