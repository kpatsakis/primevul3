static void rds_tcp_sysctl_reset(struct net *net)
{
	struct rds_tcp_connection *tc, *_tc;

	spin_lock_irq(&rds_tcp_conn_lock);
	list_for_each_entry_safe(tc, _tc, &rds_tcp_conn_list, t_tcp_node) {
		struct net *c_net = read_pnet(&tc->t_cpath->cp_conn->c_net);

		if (net != c_net || !tc->t_sock)
			continue;

		/* reconnect with new parameters */
		rds_conn_path_drop(tc->t_cpath, false);
	}
	spin_unlock_irq(&rds_tcp_conn_lock);
}
