static void rds6_tcp_tc_info(struct socket *sock, unsigned int len,
			     struct rds_info_iterator *iter,
			     struct rds_info_lengths *lens)
{
	struct rds6_info_tcp_socket tsinfo6;
	struct rds_tcp_connection *tc;
	unsigned long flags;

	spin_lock_irqsave(&rds_tcp_tc_list_lock, flags);

	if (len / sizeof(tsinfo6) < rds6_tcp_tc_count)
		goto out;

	list_for_each_entry(tc, &rds_tcp_tc_list, t_list_item) {
		struct sock *sk = tc->t_sock->sk;
		struct inet_sock *inet = inet_sk(sk);

		tsinfo6.local_addr = sk->sk_v6_rcv_saddr;
		tsinfo6.local_port = inet->inet_sport;
		tsinfo6.peer_addr = sk->sk_v6_daddr;
		tsinfo6.peer_port = inet->inet_dport;

		tsinfo6.hdr_rem = tc->t_tinc_hdr_rem;
		tsinfo6.data_rem = tc->t_tinc_data_rem;
		tsinfo6.last_sent_nxt = tc->t_last_sent_nxt;
		tsinfo6.last_expected_una = tc->t_last_expected_una;
		tsinfo6.last_seen_una = tc->t_last_seen_una;

		rds_info_copy(iter, &tsinfo6, sizeof(tsinfo6));
	}

out:
	lens->nr = rds6_tcp_tc_count;
	lens->each = sizeof(tsinfo6);

	spin_unlock_irqrestore(&rds_tcp_tc_list_lock, flags);
}
