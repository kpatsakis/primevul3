extern void record_launched_jobs(void)
{
	List steps;
	ListIterator i;
	step_loc_t *stepd;

	steps = stepd_available(conf->spooldir, conf->node_name);
	i = list_iterator_create(steps);
	while ((stepd = list_next(i))) {
		_launch_complete_add(stepd->jobid);
	}
	list_iterator_destroy(i);
	FREE_NULL_LIST(steps);
}
