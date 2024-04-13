_terminate_all_steps(uint32_t jobid, bool batch)
{
	List steps;
	ListIterator i;
	step_loc_t *stepd;
	int step_cnt  = 0;
	int fd;

	steps = stepd_available(conf->spooldir, conf->node_name);
	i = list_iterator_create(steps);
	while ((stepd = list_next(i))) {
		if (stepd->jobid != jobid) {
			/* multiple jobs expected on shared nodes */
			debug3("Step from other job: jobid=%u (this jobid=%u)",
			       stepd->jobid, jobid);
			continue;
		}

		if ((stepd->stepid == SLURM_BATCH_SCRIPT) && (!batch))
			continue;

		step_cnt++;

		fd = stepd_connect(stepd->directory, stepd->nodename,
				   stepd->jobid, stepd->stepid,
				   &stepd->protocol_version);
		if (fd == -1) {
			debug3("Unable to connect to step %u.%u",
			       stepd->jobid, stepd->stepid);
			continue;
		}

		debug2("terminate job step %u.%u", jobid, stepd->stepid);
		if (stepd_terminate(fd, stepd->protocol_version) < 0)
			debug("kill jobid=%u.%u failed: %m", jobid,
			      stepd->stepid);
		close(fd);
	}
	list_iterator_destroy(i);
	FREE_NULL_LIST(steps);
	if (step_cnt == 0)
		debug2("No steps in job %u to terminate", jobid);
	return step_cnt;
}
