static void _epilog_complete_msg_setup(
	slurm_msg_t *msg, epilog_complete_msg_t *req, uint32_t jobid, int rc)
{
	slurm_msg_t_init(msg);
	memset(req, 0, sizeof(epilog_complete_msg_t));

	req->job_id      = jobid;
	req->return_code = rc;
	req->node_name   = conf->node_name;

	msg->msg_type    = MESSAGE_EPILOG_COMPLETE;
	msg->data        = req;
}
