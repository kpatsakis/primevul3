_job_still_running(uint32_t job_id)
{
	bool         retval = false;
	List         steps;
	ListIterator i;
	step_loc_t  *s     = NULL;

	steps = stepd_available(conf->spooldir, conf->node_name);
	i = list_iterator_create(steps);
	while ((s = list_next(i))) {
		if (s->jobid == job_id) {
			int fd;
			fd = stepd_connect(s->directory, s->nodename,
					   s->jobid, s->stepid,
					   &s->protocol_version);
			if (fd == -1)
				continue;

			if (stepd_state(fd, s->protocol_version)
			    != SLURMSTEPD_NOT_RUNNING) {
				retval = true;
				close(fd);
				break;
			}
			close(fd);
		}
	}
	list_iterator_destroy(i);
	FREE_NULL_LIST(steps);

	return retval;
}
