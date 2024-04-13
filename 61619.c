static int _prolog_is_running (uint32_t jobid)
{
	int rc = 0;
	if (list_find_first (conf->prolog_running_jobs,
	                     (ListFindF) _match_jobid, &jobid))
		rc = 1;
	return (rc);
}
