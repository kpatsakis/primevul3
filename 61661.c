_signal_jobstep(uint32_t jobid, uint32_t stepid, uid_t req_uid,
		uint32_t signal)
{
	int               fd, rc = SLURM_SUCCESS;
	uid_t uid;
	uint16_t protocol_version;

	/*  There will be no stepd if the prolog is still running
	 *   Return failure so caller can retry.
	 */
	if (_prolog_is_running (jobid)) {
		info ("signal %d req for %u.%u while prolog is running."
		      " Returning failure.", signal, jobid, stepid);
		return SLURM_FAILURE;
	}

	fd = stepd_connect(conf->spooldir, conf->node_name, jobid, stepid,
			   &protocol_version);
	if (fd == -1) {
		debug("signal for nonexistent %u.%u stepd_connect failed: %m",
		      jobid, stepid);
		return ESLURM_INVALID_JOB_ID;
	}

	if ((int)(uid = stepd_get_uid(fd, protocol_version)) < 0) {
		debug("_signal_jobstep: couldn't read from the step %u.%u: %m",
		      jobid, stepid);
		rc = ESLURM_INVALID_JOB_ID;
		goto done2;
	}

	if ((req_uid != uid) && (!_slurm_authorized_user(req_uid))) {
		debug("kill req from uid %ld for job %u.%u owned by uid %ld",
		      (long) req_uid, jobid, stepid, (long) uid);
		rc = ESLURM_USER_ID_MISSING;     /* or bad in this case */
		goto done2;
	}

#ifdef HAVE_AIX
#  ifdef SIGMIGRATE
#    ifdef SIGSOUND
	/* SIGMIGRATE and SIGSOUND are used to initiate job checkpoint on AIX.
	 * These signals are not sent to the entire process group, but just a
	 * single process, namely the PMD. */
	if (signal == SIGMIGRATE || signal == SIGSOUND) {
		rc = stepd_signal_task_local(fd, protocol_version,
					     signal, 0);
		goto done2;
	}
#    endif
#  endif
#endif

	rc = stepd_signal_container(fd, protocol_version, signal);
	if (rc == -1)
		rc = ESLURMD_JOB_NOTRUNNING;

done2:
	close(fd);
	return rc;
}
