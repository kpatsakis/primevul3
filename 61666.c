_steps_completed_now(uint32_t jobid)
{
	List steps;
	ListIterator i;
	step_loc_t *stepd;
	bool rc = true;

	steps = stepd_available(conf->spooldir, conf->node_name);
	i = list_iterator_create(steps);
	while ((stepd = list_next(i))) {
		if (stepd->jobid == jobid) {
			int fd;
			fd = stepd_connect(stepd->directory, stepd->nodename,
					   stepd->jobid, stepd->stepid,
					   &stepd->protocol_version);
			if (fd == -1)
				continue;

			if (stepd_state(fd, stepd->protocol_version)
			    != SLURMSTEPD_NOT_RUNNING) {
				rc = false;
				close(fd);
				break;
			}
			close(fd);
		}
	}
	list_iterator_destroy(i);
	FREE_NULL_LIST(steps);

	return rc;
}
