void rds_tcp_restore_callbacks(struct socket *sock,
			       struct rds_tcp_connection *tc)
{
	rdsdebug("restoring sock %p callbacks from tc %p\n", sock, tc);
	write_lock_bh(&sock->sk->sk_callback_lock);

	/* done under the callback_lock to serialize with write_space */
	spin_lock(&rds_tcp_tc_list_lock);
	list_del_init(&tc->t_list_item);
#if IS_ENABLED(CONFIG_IPV6)
	rds6_tcp_tc_count--;
#endif
	if (!tc->t_cpath->cp_conn->c_isv6)
		rds_tcp_tc_count--;
	spin_unlock(&rds_tcp_tc_list_lock);

	tc->t_sock = NULL;

	sock->sk->sk_write_space = tc->t_orig_write_space;
	sock->sk->sk_data_ready = tc->t_orig_data_ready;
	sock->sk->sk_state_change = tc->t_orig_state_change;
	sock->sk->sk_user_data = NULL;

	write_unlock_bh(&sock->sk->sk_callback_lock);
}
