static void _notify_slurmctld_prolog_fini(
	uint32_t job_id, uint32_t prolog_return_code)
{
	int rc;
	slurm_msg_t req_msg;
	complete_prolog_msg_t req;

	slurm_msg_t_init(&req_msg);
	req.job_id	= job_id;
	req.prolog_rc	= prolog_return_code;

	req_msg.msg_type= REQUEST_COMPLETE_PROLOG;
	req_msg.data	= &req;

	if ((slurm_send_recv_controller_rc_msg(&req_msg, &rc) < 0) ||
	    (rc != SLURM_SUCCESS))
		error("Error sending prolog completion notification: %m");
}
