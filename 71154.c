static void __net_exit rds_tcp_exit_net(struct net *net)
{
	struct rds_tcp_net *rtn = net_generic(net, rds_tcp_netid);

	rds_tcp_kill_sock(net);

	if (rtn->rds_tcp_sysctl)
		unregister_net_sysctl_table(rtn->rds_tcp_sysctl);

	if (net != &init_net)
		kfree(rtn->ctl_table);
}
