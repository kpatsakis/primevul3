_get_suspend_job_lock(uint32_t job_id)
{
	static bool logged = false;
	int i, empty_loc = -1, rc = 0;

	slurm_mutex_lock(&suspend_mutex);
	for (i = 0; i < job_suspend_size; i++) {
		if (job_suspend_array[i] == 0) {
			empty_loc = i;
			continue;
		}
		if (job_suspend_array[i] == job_id) {
			/* another thread already a lock for this job ID */
			slurm_mutex_unlock(&suspend_mutex);
			return rc;
		}
	}

	if (empty_loc != -1) {
		/* nobody has the lock and here's an available used lock */
		job_suspend_array[empty_loc] = job_id;
		rc = 1;
	} else if (job_suspend_size < NUM_PARALLEL_SUSP_JOBS) {
		/* a new lock is available */
		job_suspend_array[job_suspend_size++] = job_id;
		rc = 1;
	} else if (!logged) {
		error("Simultaneous job suspend/resume limit reached (%d). "
		      "Configure SchedulerTimeSlice higher.",
		      NUM_PARALLEL_SUSP_JOBS);
		logged = true;
	}
	slurm_mutex_unlock(&suspend_mutex);
	return rc;
}
