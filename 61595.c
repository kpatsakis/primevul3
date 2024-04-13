_get_step_list(void)
{
	char tmp[64];
	char *step_list = NULL;
	List steps;
	ListIterator i;
	step_loc_t *stepd;

	steps = stepd_available(conf->spooldir, conf->node_name);
	i = list_iterator_create(steps);
	while ((stepd = list_next(i))) {
		int fd;
		fd = stepd_connect(stepd->directory, stepd->nodename,
				   stepd->jobid, stepd->stepid,
				   &stepd->protocol_version);
		if (fd == -1)
			continue;

		if (stepd_state(fd, stepd->protocol_version)
		    == SLURMSTEPD_NOT_RUNNING) {
			debug("stale domain socket for stepd %u.%u ",
			      stepd->jobid, stepd->stepid);
			close(fd);
			continue;
		}
		close(fd);

		if (step_list)
			xstrcat(step_list, ", ");
		if (stepd->stepid == NO_VAL) {
			snprintf(tmp, sizeof(tmp), "%u",
				 stepd->jobid);
			xstrcat(step_list, tmp);
		} else {
			snprintf(tmp, sizeof(tmp), "%u.%u",
				 stepd->jobid, stepd->stepid);
			xstrcat(step_list, tmp);
		}
	}
	list_iterator_destroy(i);
	FREE_NULL_LIST(steps);

	if (step_list == NULL)
		xstrcat(step_list, "NONE");
	return step_list;
}
