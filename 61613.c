_load_job_limits(void)
{
	List steps;
	ListIterator step_iter;
	step_loc_t *stepd;
	int fd;
	job_mem_limits_t *job_limits_ptr;
	slurmstepd_mem_info_t stepd_mem_info;

	if (!job_limits_list)
		job_limits_list = list_create(_job_limits_free);
	job_limits_loaded = true;

	steps = stepd_available(conf->spooldir, conf->node_name);
	step_iter = list_iterator_create(steps);
	while ((stepd = list_next(step_iter))) {
		job_limits_ptr = list_find_first(job_limits_list,
						 _step_limits_match, stepd);
		if (job_limits_ptr)	/* already processed */
			continue;
		fd = stepd_connect(stepd->directory, stepd->nodename,
				   stepd->jobid, stepd->stepid,
				   &stepd->protocol_version);
		if (fd == -1)
			continue;	/* step completed */

		if (stepd_get_mem_limits(fd, stepd->protocol_version,
					  &stepd_mem_info) != SLURM_SUCCESS) {
			error("Error reading step %u.%u memory limits from "
			      "slurmstepd",
			      stepd->jobid, stepd->stepid);
			close(fd);
			continue;
		}


		if ((stepd_mem_info.job_mem_limit
		     || stepd_mem_info.step_mem_limit)) {
			/* create entry for this job */
			job_limits_ptr = xmalloc(sizeof(job_mem_limits_t));
			job_limits_ptr->job_id   = stepd->jobid;
			job_limits_ptr->step_id  = stepd->stepid;
			job_limits_ptr->job_mem  =
				stepd_mem_info.job_mem_limit;
			job_limits_ptr->step_mem =
				stepd_mem_info.step_mem_limit;
#if _LIMIT_INFO
			info("RecLim step:%u.%u job_mem:%u step_mem:%u",
			     job_limits_ptr->job_id, job_limits_ptr->step_id,
			     job_limits_ptr->job_mem,
			     job_limits_ptr->step_mem);
#endif
			list_append(job_limits_list, job_limits_ptr);
		}
		close(fd);
	}
	list_iterator_destroy(step_iter);
	FREE_NULL_LIST(steps);
}
