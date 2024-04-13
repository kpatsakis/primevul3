static uid_t _get_job_uid(uint32_t jobid)
{
	List steps;
	ListIterator i;
	step_loc_t *stepd;
	uid_t uid = -1;
	int fd;

	steps = stepd_available(conf->spooldir, conf->node_name);
	i = list_iterator_create(steps);
	while ((stepd = list_next(i))) {
		if (stepd->jobid != jobid) {
			/* multiple jobs expected on shared nodes */
			continue;
		}
		fd = stepd_connect(stepd->directory, stepd->nodename,
				   stepd->jobid, stepd->stepid,
				   &stepd->protocol_version);
		if (fd == -1) {
			debug3("Unable to connect to step %u.%u",
			       stepd->jobid, stepd->stepid);
			continue;
		}
		uid = stepd_get_uid(fd, stepd->protocol_version);

		close(fd);
		if ((int)uid < 0) {
			debug("stepd_get_uid failed %u.%u: %m",
			      stepd->jobid, stepd->stepid);
			continue;
		}
		break;
	}
	list_iterator_destroy(i);
	FREE_NULL_LIST(steps);

	return uid;
}
