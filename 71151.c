static void rds_tcp_accept_worker(struct work_struct *work)
{
	struct rds_tcp_net *rtn = container_of(work,
					       struct rds_tcp_net,
					       rds_tcp_accept_w);

	while (rds_tcp_accept_one(rtn->rds_tcp_listen_sock) == 0)
		cond_resched();
}
