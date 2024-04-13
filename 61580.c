_check_job_credential(launch_tasks_request_msg_t *req, uid_t uid,
		      int node_id, hostset_t *step_hset,
		      uint16_t protocol_version)
{
	slurm_cred_arg_t arg;
	hostset_t	s_hset = NULL;
	bool		user_ok = _slurm_authorized_user(uid);
	bool		verified = true;
	int		host_index = -1;
	int		rc;
	slurm_cred_t    *cred = req->cred;
	uint32_t	jobid = req->job_id;
	uint32_t	stepid = req->job_step_id;
	int		tasks_to_launch = req->tasks_to_launch[node_id];
	uint32_t	job_cpus = 0, step_cpus = 0;

	/*
	 * First call slurm_cred_verify() so that all valid
	 * credentials are checked
	 */
	rc = slurm_cred_verify(conf->vctx, cred, &arg, protocol_version);
	if (rc < 0) {
		verified = false;
		if ((!user_ok) || (errno != ESLURMD_INVALID_JOB_CREDENTIAL))
			return SLURM_ERROR;
		else {
			debug("_check_job_credential slurm_cred_verify failed:"
			      " %m, but continuing anyway.");
		}
	}

	/* If uid is the SlurmUser or root and the credential is bad,
	 * then do not attempt validating the credential */
	if (!verified) {
		*step_hset = NULL;
		if (rc >= 0) {
			if ((s_hset = hostset_create(arg.step_hostlist)))
				*step_hset = s_hset;
			slurm_cred_free_args(&arg);
		}
		return SLURM_SUCCESS;
	}

	if ((arg.jobid != jobid) || (arg.stepid != stepid)) {
		error("job credential for %u.%u, expected %u.%u",
		      arg.jobid, arg.stepid, jobid, stepid);
		goto fail;
	}

	if (arg.uid != uid) {
		error("job credential created for uid %ld, expected %ld",
		      (long) arg.uid, (long) uid);
		goto fail;
	}

	/*
	 * Check that credential is valid for this host
	 */
	if (!(s_hset = hostset_create(arg.step_hostlist))) {
		error("Unable to parse credential hostlist: `%s'",
		      arg.step_hostlist);
		goto fail;
	}

	if (!hostset_within(s_hset, conf->node_name)) {
		error("Invalid job %u.%u credential for user %u: "
		      "host %s not in hostset %s",
		      arg.jobid, arg.stepid, arg.uid,
		      conf->node_name, arg.step_hostlist);
		goto fail;
	}

	if ((arg.job_nhosts > 0) && (tasks_to_launch > 0)) {
		uint32_t hi, i, i_first_bit=0, i_last_bit=0, j;
		bool cpu_log = slurm_get_debug_flags() & DEBUG_FLAG_CPU_BIND;

#ifdef HAVE_FRONT_END
		host_index = 0;	/* It is always 0 for front end systems */
#else
		hostset_t j_hset;
		/* Determine the CPU count based upon this node's index into
		 * the _job's_ allocation (job's hostlist and core_bitmap) */
		if (!(j_hset = hostset_create(arg.job_hostlist))) {
			error("Unable to parse credential hostlist: `%s'",
			      arg.job_hostlist);
			goto fail;
		}
		host_index = hostset_find(j_hset, conf->node_name);
		hostset_destroy(j_hset);

		if ((host_index < 0) || (host_index >= arg.job_nhosts)) {
			error("job cr credential invalid host_index %d for "
			      "job %u", host_index, arg.jobid);
			goto fail;
		}
#endif

		if (cpu_log) {
			char *per_job = "", *per_step = "";
			uint32_t job_mem  = arg.job_mem_limit;
			uint32_t step_mem = arg.step_mem_limit;
			if (job_mem & MEM_PER_CPU) {
				job_mem &= (~MEM_PER_CPU);
				per_job = "_per_CPU";
			}
			if (step_mem & MEM_PER_CPU) {
				step_mem &= (~MEM_PER_CPU);
				per_step = "_per_CPU";
			}
			info("====================");
			info("step_id:%u.%u job_mem:%uMB%s step_mem:%uMB%s",
			     arg.jobid, arg.stepid, job_mem, per_job,
			     step_mem, per_step);
		}

		hi = host_index + 1;	/* change from 0-origin to 1-origin */
		for (i=0; hi; i++) {
			if (hi > arg.sock_core_rep_count[i]) {
				i_first_bit += arg.sockets_per_node[i] *
					       arg.cores_per_socket[i] *
					       arg.sock_core_rep_count[i];
				hi -= arg.sock_core_rep_count[i];
			} else {
				i_first_bit += arg.sockets_per_node[i] *
					       arg.cores_per_socket[i] *
					       (hi - 1);
				i_last_bit = i_first_bit +
					     arg.sockets_per_node[i] *
					     arg.cores_per_socket[i];
				break;
			}
		}
		/* Now count the allocated processors */
		for (i=i_first_bit, j=0; i<i_last_bit; i++, j++) {
			char *who_has = NULL;
			if (bit_test(arg.job_core_bitmap, i)) {
				job_cpus++;
				who_has = "Job";
			}
			if (bit_test(arg.step_core_bitmap, i)) {
				step_cpus++;
				who_has = "Step";
			}
			if (cpu_log && who_has) {
				info("JobNode[%u] CPU[%u] %s alloc",
				     host_index, j, who_has);
			}
		}
		if (cpu_log)
			info("====================");
		if (step_cpus == 0) {
			error("cons_res: zero processors allocated to step");
			step_cpus = 1;
		}
		/* NOTE: step_cpus is the count of allocated resources
		 * (typically cores). Convert to CPU count as needed */
		if (i_last_bit <= i_first_bit)
			error("step credential has no CPUs selected");
		else {
			i = conf->cpus / (i_last_bit - i_first_bit);
			if (i > 1) {
				if (cpu_log)
					info("Scaling CPU count by factor of "
					     "%d (%u/(%u-%u))",
					     i, conf->cpus,
					     i_last_bit, i_first_bit);
				step_cpus *= i;
				job_cpus *= i;
			}
		}
		if (tasks_to_launch > step_cpus) {
			/* This is expected with the --overcommit option
			 * or hyperthreads */
			debug("cons_res: More than one tasks per logical "
			      "processor (%d > %u) on host [%u.%u %ld %s] ",
			      tasks_to_launch, step_cpus, arg.jobid,
			      arg.stepid, (long) arg.uid, arg.step_hostlist);
		}
	} else {
		step_cpus = 1;
		job_cpus  = 1;
	}

	/* Overwrite any memory limits in the RPC with contents of the
	 * memory limit within the credential.
	 * Reset the CPU count on this node to correct value. */
	if (arg.step_mem_limit) {
		if (arg.step_mem_limit & MEM_PER_CPU) {
			req->step_mem_lim  = arg.step_mem_limit &
					     (~MEM_PER_CPU);
			req->step_mem_lim *= step_cpus;
		} else
			req->step_mem_lim  = arg.step_mem_limit;
	} else {
		if (arg.job_mem_limit & MEM_PER_CPU) {
			req->step_mem_lim  = arg.job_mem_limit &
					     (~MEM_PER_CPU);
			req->step_mem_lim *= job_cpus;
		} else
			req->step_mem_lim  = arg.job_mem_limit;
	}
	if (arg.job_mem_limit & MEM_PER_CPU) {
		req->job_mem_lim  = arg.job_mem_limit & (~MEM_PER_CPU);
		req->job_mem_lim *= job_cpus;
	} else
		req->job_mem_lim  = arg.job_mem_limit;
	req->job_core_spec = arg.job_core_spec;
	req->node_cpus = step_cpus;
#if 0
	info("%u.%u node_id:%d mem orig:%u cpus:%u limit:%u",
	     jobid, stepid, node_id, arg.job_mem_limit,
	     step_cpus, req->job_mem_lim);
#endif

	*step_hset = s_hset;
	slurm_cred_free_args(&arg);
	return SLURM_SUCCESS;

    fail:
	if (s_hset)
		hostset_destroy(s_hset);
	*step_hset = NULL;
	slurm_cred_free_args(&arg);
	slurm_seterrno_ret(ESLURMD_INVALID_JOB_CREDENTIAL);
}
