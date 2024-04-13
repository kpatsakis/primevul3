static int _waiter_init (uint32_t jobid)
{
	if (!waiters)
		waiters = list_create((ListDelF) _waiter_destroy);

	/*
	 *  Exit this thread if another thread is waiting on job
	 */
	if (list_find_first (waiters, (ListFindF) _find_waiter, &jobid))
		return SLURM_ERROR;
	else
		list_append(waiters, _waiter_create(jobid));

	return (SLURM_SUCCESS);
}
