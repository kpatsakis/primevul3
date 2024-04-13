_valid_sbcast_cred(file_bcast_msg_t *req, uid_t req_uid, uint16_t block_no,
		   uint32_t *job_id)
{
	int rc = SLURM_SUCCESS;
	char *nodes = NULL;
	hostset_t hset = NULL;

	*job_id = NO_VAL;
	rc = extract_sbcast_cred(conf->vctx, req->cred, block_no,
				 job_id, &nodes);
	if (rc != 0) {
		error("Security violation: Invalid sbcast_cred from uid %d",
		      req_uid);
		return ESLURMD_INVALID_JOB_CREDENTIAL;
	}

	if (!(hset = hostset_create(nodes))) {
		error("Unable to parse sbcast_cred hostlist %s", nodes);
		rc = ESLURMD_INVALID_JOB_CREDENTIAL;
	} else if (!hostset_within(hset, conf->node_name)) {
		error("Security violation: sbcast_cred from %d has "
		      "bad hostset %s", req_uid, nodes);
		rc = ESLURMD_INVALID_JOB_CREDENTIAL;
	}
	if (hset)
		hostset_destroy(hset);
	xfree(nodes);

	/* print_sbcast_cred(req->cred); */

	return rc;
}
