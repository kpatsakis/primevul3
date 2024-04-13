_abort_step(uint32_t job_id, uint32_t step_id)
{
	step_complete_msg_t resp;
	slurm_msg_t resp_msg;
	slurm_msg_t_init(&resp_msg);
	int rc, rc2;

	resp.job_id       = job_id;
	resp.job_step_id  = step_id;
	resp.range_first  = 0;
	resp.range_last   = 0;
	resp.step_rc      = 1;
	resp.jobacct      = jobacctinfo_create(NULL);
	resp_msg.msg_type = REQUEST_STEP_COMPLETE;
	resp_msg.data     = &resp;
	rc2 = slurm_send_recv_controller_rc_msg(&resp_msg, &rc);
	/* Note: we are ignoring the RPC return code */
	jobacctinfo_destroy(resp.jobacct);
	return rc2;
}
