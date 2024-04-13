static void rds_tcp_tc_info(struct socket *rds_sock, unsigned int len,
			    struct rds_info_iterator *iter,
			    struct rds_info_lengths *lens)
{
	struct rds_info_tcp_socket tsinfo;
	struct rds_tcp_connection *tc;
	unsigned long flags;

	spin_lock_irqsave(&rds_tcp_tc_list_lock, flags);

	if (len / sizeof(tsinfo) < rds_tcp_tc_count)
		goto out;

	list_for_each_entry(tc, &rds_tcp_tc_list, t_list_item) {
		struct inet_sock *inet = inet_sk(tc->t_sock->sk);

		if (tc->t_cpath->cp_conn->c_isv6)
			continue;

		tsinfo.local_addr = inet->inet_saddr;
		tsinfo.local_port = inet->inet_sport;
		tsinfo.peer_addr = inet->inet_daddr;
		tsinfo.peer_port = inet->inet_dport;

		tsinfo.hdr_rem = tc->t_tinc_hdr_rem;
		tsinfo.data_rem = tc->t_tinc_data_rem;
		tsinfo.last_sent_nxt = tc->t_last_sent_nxt;
		tsinfo.last_expected_una = tc->t_last_expected_una;
		tsinfo.last_seen_una = tc->t_last_seen_una;
		tsinfo.tos = tc->t_cpath->cp_conn->c_tos;

		rds_info_copy(iter, &tsinfo, sizeof(tsinfo));
	}

out:
	lens->nr = rds_tcp_tc_count;
	lens->each = sizeof(tsinfo);

	spin_unlock_irqrestore(&rds_tcp_tc_list_lock, flags);
}
