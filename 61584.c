_enforce_job_mem_limit(void)
{
	List steps;
	ListIterator step_iter, job_limits_iter;
	job_mem_limits_t *job_limits_ptr;
	step_loc_t *stepd;
	int fd, i, job_inx, job_cnt;
	uint16_t vsize_factor;
	uint64_t step_rss, step_vsize;
	job_step_id_msg_t acct_req;
	job_step_stat_t *resp = NULL;
	struct job_mem_info {
		uint32_t job_id;
		uint32_t mem_limit;	/* MB */
		uint32_t mem_used;	/* MB */
		uint32_t vsize_limit;	/* MB */
		uint32_t vsize_used;	/* MB */
	};
	struct job_mem_info *job_mem_info_ptr = NULL;

	/* If users have configured MemLimitEnforce=no
	 * in their slurm.conf keep going.
	 */
	if (conf->mem_limit_enforce == false)
		return;

	slurm_mutex_lock(&job_limits_mutex);
	if (!job_limits_loaded)
		_load_job_limits();
	if (list_count(job_limits_list) == 0) {
		slurm_mutex_unlock(&job_limits_mutex);
		return;
	}

	/* Build table of job limits, use highest mem limit recorded */
	job_mem_info_ptr = xmalloc((list_count(job_limits_list) + 1) *
				   sizeof(struct job_mem_info));
	job_cnt = 0;
	job_limits_iter = list_iterator_create(job_limits_list);
	while ((job_limits_ptr = list_next(job_limits_iter))) {
		if (job_limits_ptr->job_mem == 0) 	/* no job limit */
			continue;
		for (i=0; i<job_cnt; i++) {
			if (job_mem_info_ptr[i].job_id !=
			    job_limits_ptr->job_id)
				continue;
			job_mem_info_ptr[i].mem_limit = MAX(
				job_mem_info_ptr[i].mem_limit,
				job_limits_ptr->job_mem);
			break;
		}
		if (i < job_cnt)	/* job already found & recorded */
			continue;
		job_mem_info_ptr[job_cnt].job_id    = job_limits_ptr->job_id;
		job_mem_info_ptr[job_cnt].mem_limit = job_limits_ptr->job_mem;
		job_cnt++;
	}
	list_iterator_destroy(job_limits_iter);
	slurm_mutex_unlock(&job_limits_mutex);

	vsize_factor = slurm_get_vsize_factor();
	for (i=0; i<job_cnt; i++) {
		job_mem_info_ptr[i].vsize_limit = job_mem_info_ptr[i].
			mem_limit;
		job_mem_info_ptr[i].vsize_limit *= (vsize_factor / 100.0);
	}

	steps = stepd_available(conf->spooldir, conf->node_name);
	step_iter = list_iterator_create(steps);
	while ((stepd = list_next(step_iter))) {
		for (job_inx=0; job_inx<job_cnt; job_inx++) {
			if (job_mem_info_ptr[job_inx].job_id == stepd->jobid)
				break;
		}
		if (job_inx >= job_cnt)
			continue;	/* job/step not being tracked */

		fd = stepd_connect(stepd->directory, stepd->nodename,
				   stepd->jobid, stepd->stepid,
				   &stepd->protocol_version);
		if (fd == -1)
			continue;	/* step completed */
		acct_req.job_id  = stepd->jobid;
		acct_req.step_id = stepd->stepid;
		resp = xmalloc(sizeof(job_step_stat_t));

		if ((!stepd_stat_jobacct(
			     fd, stepd->protocol_version,
			     &acct_req, resp)) &&
		    (resp->jobacct)) {
			/* resp->jobacct is NULL if account is disabled */
			jobacctinfo_getinfo((struct jobacctinfo *)
					    resp->jobacct,
					    JOBACCT_DATA_TOT_RSS,
					    &step_rss,
					    stepd->protocol_version);
			jobacctinfo_getinfo((struct jobacctinfo *)
					    resp->jobacct,
					    JOBACCT_DATA_TOT_VSIZE,
					    &step_vsize,
					    stepd->protocol_version);
#if _LIMIT_INFO
			info("Step:%u.%u RSS:%"PRIu64" KB VSIZE:%"PRIu64" KB",
			     stepd->jobid, stepd->stepid,
			     step_rss, step_vsize);
#endif
			step_rss /= 1024;	/* KB to MB */
			step_rss = MAX(step_rss, 1);
			job_mem_info_ptr[job_inx].mem_used += step_rss;
			step_vsize /= 1024;	/* KB to MB */
			step_vsize = MAX(step_vsize, 1);
			job_mem_info_ptr[job_inx].vsize_used += step_vsize;
		}
		slurm_free_job_step_stat(resp);
		close(fd);
	}
	list_iterator_destroy(step_iter);
	FREE_NULL_LIST(steps);

	for (i=0; i<job_cnt; i++) {
		if (job_mem_info_ptr[i].mem_used == 0) {
			/* no steps found,
			 * purge records for all steps of this job */
			slurm_mutex_lock(&job_limits_mutex);
			list_delete_all(job_limits_list, _job_limits_match,
					&job_mem_info_ptr[i].job_id);
			slurm_mutex_unlock(&job_limits_mutex);
			break;
		}

		if ((job_mem_info_ptr[i].mem_limit != 0) &&
		    (job_mem_info_ptr[i].mem_used >
		     job_mem_info_ptr[i].mem_limit)) {
			info("Job %u exceeded memory limit (%u>%u), "
			     "cancelling it", job_mem_info_ptr[i].job_id,
			     job_mem_info_ptr[i].mem_used,
			     job_mem_info_ptr[i].mem_limit);
			_cancel_step_mem_limit(job_mem_info_ptr[i].job_id,
					       NO_VAL);
		} else if ((job_mem_info_ptr[i].vsize_limit != 0) &&
			   (job_mem_info_ptr[i].vsize_used >
			    job_mem_info_ptr[i].vsize_limit)) {
			info("Job %u exceeded virtual memory limit (%u>%u), "
			     "cancelling it", job_mem_info_ptr[i].job_id,
			     job_mem_info_ptr[i].vsize_used,
			     job_mem_info_ptr[i].vsize_limit);
			_cancel_step_mem_limit(job_mem_info_ptr[i].job_id,
					       NO_VAL);
		}
	}
	xfree(job_mem_info_ptr);
}
