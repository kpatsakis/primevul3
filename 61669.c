_unlock_suspend_job(uint32_t job_id)
{
	int i;
	slurm_mutex_lock(&suspend_mutex);
	for (i = 0; i < job_suspend_size; i++) {
		if (job_suspend_array[i] == job_id)
			job_suspend_array[i] = 0;
	}
	slurm_mutex_unlock(&suspend_mutex);
}
