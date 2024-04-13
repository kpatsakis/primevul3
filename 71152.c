static void rds_tcp_destroy_conns(void)
{
	struct rds_tcp_connection *tc, *_tc;
	LIST_HEAD(tmp_list);

	/* avoid calling conn_destroy with irqs off */
	spin_lock_irq(&rds_tcp_conn_lock);
	list_for_each_entry_safe(tc, _tc, &rds_tcp_conn_list, t_tcp_node) {
		if (!list_has_conn(&tmp_list, tc->t_cpath->cp_conn))
			list_move_tail(&tc->t_tcp_node, &tmp_list);
	}
	spin_unlock_irq(&rds_tcp_conn_lock);

	list_for_each_entry_safe(tc, _tc, &tmp_list, t_tcp_node)
		rds_conn_destroy(tc->t_cpath->cp_conn);
}
