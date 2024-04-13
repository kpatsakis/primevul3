_run_spank_job_script (const char *mode, char **env, uint32_t job_id, uid_t uid)
{
	pid_t cpid;
	int status = 0, timeout;
	int pfds[2];

	if (pipe (pfds) < 0) {
		error ("_run_spank_job_script: pipe: %m");
		return (-1);
	}

	fd_set_close_on_exec (pfds[1]);

	debug ("Calling %s spank %s", conf->stepd_loc, mode);
	if ((cpid = fork ()) < 0) {
		error ("executing spank %s: %m", mode);
		return (-1);
	}
	if (cpid == 0) {
		/* Run slurmstepd spank [prolog|epilog] */
		char *argv[4] = {
			(char *) conf->stepd_loc,
			"spank",
			(char *) mode,
			NULL };

		/* container_g_add_pid needs to be called in the
		   forked process part of the fork to avoid a race
		   condition where if this process makes a file or
		   detacts itself from a child before we add the pid
		   to the container in the parent of the fork.
		*/
		if (container_g_add_pid(job_id, getpid(), getuid())
		    != SLURM_SUCCESS)
			error("container_g_add_pid(%u): %m", job_id);

		if (dup2 (pfds[0], STDIN_FILENO) < 0)
			fatal ("dup2: %m");
#ifdef SETPGRP_TWO_ARGS
		setpgrp(0, 0);
#else
		setpgrp();
#endif
		if (conf->chos_loc && !access(conf->chos_loc, X_OK))
			execve(conf->chos_loc, argv, env);
		else
			execve(argv[0], argv, env);
		error ("execve(%s): %m", argv[0]);
		exit (127);
	}

	close (pfds[0]);

	if (_send_slurmd_conf_lite (pfds[1], conf) < 0)
		error ("Failed to send slurmd conf to slurmstepd\n");
	close (pfds[1]);

	timeout = MAX(slurm_get_prolog_timeout(), 120); /* 120 secs in v15.08 */
	if (waitpid_timeout (mode, cpid, &status, timeout) < 0) {
		error ("spank/%s timed out after %u secs", mode, timeout);
		return (-1);
	}

	if (status)
		error ("spank/%s returned status 0x%04x", mode, status);

	/*
	 *  No longer need SPANK option env vars in environment
	 */
	spank_clear_remote_options_env (env);

	return (status);
}
