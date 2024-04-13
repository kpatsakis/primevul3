_wait_state_completed(uint32_t jobid, int max_delay)
{
	int i;

	for (i=0; i<max_delay; i++) {
		if (_steps_completed_now(jobid))
			break;
		sleep(1);
	}
	if (i >= max_delay)
		error("timed out waiting for job %u to complete", jobid);
}
