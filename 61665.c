static int _step_limits_match(void *x, void *key)
{
	job_mem_limits_t *job_limits_ptr = (job_mem_limits_t *) x;
	step_loc_t *step_ptr = (step_loc_t *) key;

	if ((job_limits_ptr->job_id  == step_ptr->jobid) &&
	    (job_limits_ptr->step_id == step_ptr->stepid))
		return 1;
	return 0;
}
