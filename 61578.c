_callerid_find_job(callerid_conn_t conn, uint32_t *job_id)
{
	ino_t inode;
	pid_t pid;
	int rc;

	rc = callerid_find_inode_by_conn(conn, &inode);
	if (rc != SLURM_SUCCESS) {
		debug3("network_callerid inode not found");
		return ESLURM_INVALID_JOB_ID;
	}
	debug3("network_callerid found inode %lu", (long unsigned int)inode);

	rc = find_pid_by_inode(&pid, inode);
	if (rc != SLURM_SUCCESS) {
		debug3("network_callerid process not found");
		return ESLURM_INVALID_JOB_ID;
	}
	debug3("network_callerid found process %d", (pid_t)pid);

	rc = slurm_pid2jobid(pid, job_id);
	if (rc != SLURM_SUCCESS) {
		debug3("network_callerid job not found");
		return ESLURM_INVALID_JOB_ID;
	}
	debug3("network_callerid found job %u", *job_id);
	return SLURM_SUCCESS;
}
