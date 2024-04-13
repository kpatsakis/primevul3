_pause_for_job_completion (uint32_t job_id, char *nodes, int max_time)
{
	int sec = 0;
	int pause = 1;
	bool rc = false;

	while ((sec < max_time) || (max_time == 0)) {
		rc = _job_still_running (job_id);
		if (!rc)
			break;
		if ((max_time == 0) && (sec > 1)) {
			_terminate_all_steps(job_id, true);
		}
		if (sec > 10) {
			/* Reduce logging frequency about unkillable tasks */
			if (max_time)
				pause = MIN((max_time - sec), 10);
			else
				pause = 10;
		}
		sleep(pause);
		sec += pause;
	}

	/*
	 * Return true if job is NOT running
	 */
	return (!rc);
}
