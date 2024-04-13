_cancel_step_mem_limit(uint32_t job_id, uint32_t step_id)
{
	slurm_msg_t msg;
	job_notify_msg_t notify_req;
	job_step_kill_msg_t kill_req;

	/* NOTE: Batch jobs may have no srun to get this message */
	slurm_msg_t_init(&msg);
	notify_req.job_id      = job_id;
	notify_req.job_step_id = step_id;
	notify_req.message     = "Exceeded job memory limit";
	msg.msg_type    = REQUEST_JOB_NOTIFY;
	msg.data        = &notify_req;
	slurm_send_only_controller_msg(&msg);

	memset(&kill_req, 0, sizeof(job_step_kill_msg_t));
	kill_req.job_id      = job_id;
	kill_req.job_step_id = step_id;
	kill_req.signal      = SIGKILL;
	kill_req.flags       = (uint16_t) 0;
	msg.msg_type    = REQUEST_CANCEL_JOB_STEP;
	msg.data        = &kill_req;
	slurm_send_only_controller_msg(&msg);
}
