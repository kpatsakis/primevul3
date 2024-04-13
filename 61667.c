static void _sync_messages_kill(kill_job_msg_t *req)
{
	int host_cnt, host_inx;
	char *host;
	hostset_t hosts;
	int epilog_msg_time;

	hosts = hostset_create(req->nodes);
	host_cnt = hostset_count(hosts);
	if (host_cnt <= 64)
		goto fini;
	if (conf->hostname == NULL)
		goto fini;	/* should never happen */

	for (host_inx=0; host_inx<host_cnt; host_inx++) {
		host = hostset_shift(hosts);
		if (host == NULL)
			break;
		if (xstrcmp(host, conf->node_name) == 0) {
			free(host);
			break;
		}
		free(host);
	}
	epilog_msg_time = slurm_get_epilog_msg_time();
	_delay_rpc(host_inx, host_cnt, epilog_msg_time);

 fini:	hostset_destroy(hosts);
}
