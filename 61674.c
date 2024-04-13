static int _waiter_complete (uint32_t jobid)
{
	return (list_delete_all (waiters, (ListFindF) _find_waiter, &jobid));
}
