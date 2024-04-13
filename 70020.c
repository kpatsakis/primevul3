static int rds_cmsg_recv(struct rds_incoming *inc, struct msghdr *msg,
			 struct rds_sock *rs)
{
	int ret = 0;

	if (inc->i_rdma_cookie) {
		ret = put_cmsg(msg, SOL_RDS, RDS_CMSG_RDMA_DEST,
				sizeof(inc->i_rdma_cookie), &inc->i_rdma_cookie);
		if (ret)
			goto out;
	}

	if ((inc->i_rx_tstamp != 0) &&
	    sock_flag(rds_rs_to_sk(rs), SOCK_RCVTSTAMP)) {
		struct __kernel_old_timeval tv = ns_to_kernel_old_timeval(inc->i_rx_tstamp);

		if (!sock_flag(rds_rs_to_sk(rs), SOCK_TSTAMP_NEW)) {
			ret = put_cmsg(msg, SOL_SOCKET, SO_TIMESTAMP_OLD,
				       sizeof(tv), &tv);
		} else {
			struct __kernel_sock_timeval sk_tv;

			sk_tv.tv_sec = tv.tv_sec;
			sk_tv.tv_usec = tv.tv_usec;

			ret = put_cmsg(msg, SOL_SOCKET, SO_TIMESTAMP_NEW,
				       sizeof(sk_tv), &sk_tv);
		}

		if (ret)
			goto out;
	}

	if (rs->rs_rx_traces) {
		struct rds_cmsg_rx_trace t;
		int i, j;

		memset(&t, 0, sizeof(t));
		inc->i_rx_lat_trace[RDS_MSG_RX_CMSG] = local_clock();
		t.rx_traces =  rs->rs_rx_traces;
		for (i = 0; i < rs->rs_rx_traces; i++) {
			j = rs->rs_rx_trace[i];
			t.rx_trace_pos[i] = j;
			t.rx_trace[i] = inc->i_rx_lat_trace[j + 1] -
					  inc->i_rx_lat_trace[j];
		}

		ret = put_cmsg(msg, SOL_RDS, RDS_CMSG_RXPATH_LATENCY,
			       sizeof(t), &t);
		if (ret)
			goto out;
	}

out:
	return ret;
}
