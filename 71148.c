static bool list_has_conn(struct list_head *list, struct rds_connection *conn)
{
	struct rds_tcp_connection *tc, *_tc;

	list_for_each_entry_safe(tc, _tc, list, t_tcp_node) {
		if (tc->t_cpath->cp_conn == conn)
			return true;
	}
	return false;
}
