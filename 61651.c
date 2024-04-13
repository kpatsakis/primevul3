_rpc_update_time(slurm_msg_t *msg)
{
	int   rc      = SLURM_SUCCESS;
	uid_t req_uid = g_slurm_auth_get_uid(msg->auth_cred,
					     conf->auth_info);

	if ((req_uid != conf->slurm_user_id) && (req_uid != 0)) {
		rc = ESLURM_USER_ID_MISSING;
		error("Security violation, uid %d can't update time limit",
		      req_uid);
		goto done;
	}

/* 	if (shm_update_job_timelimit(req->job_id, req->expiration_time) < 0) { */
/* 		error("updating lifetime for job %u: %m", req->job_id); */
/* 		rc = ESLURM_INVALID_JOB_ID; */
/* 	} else */
/* 		debug("reset job %u lifetime", req->job_id); */

    done:
	slurm_send_rc_msg(msg, rc);
}
