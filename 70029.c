int rds_recvmsg(struct socket *sock, struct msghdr *msg, size_t size,
		int msg_flags)
{
	struct sock *sk = sock->sk;
	struct rds_sock *rs = rds_sk_to_rs(sk);
	long timeo;
	int ret = 0, nonblock = msg_flags & MSG_DONTWAIT;
	DECLARE_SOCKADDR(struct sockaddr_in6 *, sin6, msg->msg_name);
	DECLARE_SOCKADDR(struct sockaddr_in *, sin, msg->msg_name);
	struct rds_incoming *inc = NULL;

	/* udp_recvmsg()->sock_recvtimeo() gets away without locking too.. */
	timeo = sock_rcvtimeo(sk, nonblock);

	rdsdebug("size %zu flags 0x%x timeo %ld\n", size, msg_flags, timeo);

	if (msg_flags & MSG_OOB)
		goto out;
	if (msg_flags & MSG_ERRQUEUE)
		return sock_recv_errqueue(sk, msg, size, SOL_IP, IP_RECVERR);

	while (1) {
		/* If there are pending notifications, do those - and nothing else */
		if (!list_empty(&rs->rs_notify_queue)) {
			ret = rds_notify_queue_get(rs, msg);
			break;
		}

		if (rs->rs_cong_notify) {
			ret = rds_notify_cong(rs, msg);
			break;
		}

		if (!rds_next_incoming(rs, &inc)) {
			if (nonblock) {
				bool reaped = rds_recvmsg_zcookie(rs, msg);

				ret = reaped ?  0 : -EAGAIN;
				break;
			}

			timeo = wait_event_interruptible_timeout(*sk_sleep(sk),
					(!list_empty(&rs->rs_notify_queue) ||
					 rs->rs_cong_notify ||
					 rds_next_incoming(rs, &inc)), timeo);
			rdsdebug("recvmsg woke inc %p timeo %ld\n", inc,
				 timeo);
			if (timeo > 0 || timeo == MAX_SCHEDULE_TIMEOUT)
				continue;

			ret = timeo;
			if (ret == 0)
				ret = -ETIMEDOUT;
			break;
		}

		rdsdebug("copying inc %p from %pI6c:%u to user\n", inc,
			 &inc->i_conn->c_faddr,
			 ntohs(inc->i_hdr.h_sport));
		ret = inc->i_conn->c_trans->inc_copy_to_user(inc, &msg->msg_iter);
		if (ret < 0)
			break;

		/*
		 * if the message we just copied isn't at the head of the
		 * recv queue then someone else raced us to return it, try
		 * to get the next message.
		 */
		if (!rds_still_queued(rs, inc, !(msg_flags & MSG_PEEK))) {
			rds_inc_put(inc);
			inc = NULL;
			rds_stats_inc(s_recv_deliver_raced);
			iov_iter_revert(&msg->msg_iter, ret);
			continue;
		}

		if (ret < be32_to_cpu(inc->i_hdr.h_len)) {
			if (msg_flags & MSG_TRUNC)
				ret = be32_to_cpu(inc->i_hdr.h_len);
			msg->msg_flags |= MSG_TRUNC;
		}

		if (rds_cmsg_recv(inc, msg, rs)) {
			ret = -EFAULT;
			goto out;
		}
		rds_recvmsg_zcookie(rs, msg);

		rds_stats_inc(s_recv_delivered);

		if (msg->msg_name) {
			if (ipv6_addr_v4mapped(&inc->i_saddr)) {
				sin = (struct sockaddr_in *)msg->msg_name;

				sin->sin_family = AF_INET;
				sin->sin_port = inc->i_hdr.h_sport;
				sin->sin_addr.s_addr =
				    inc->i_saddr.s6_addr32[3];
				memset(sin->sin_zero, 0, sizeof(sin->sin_zero));
				msg->msg_namelen = sizeof(*sin);
			} else {
				sin6 = (struct sockaddr_in6 *)msg->msg_name;

				sin6->sin6_family = AF_INET6;
				sin6->sin6_port = inc->i_hdr.h_sport;
				sin6->sin6_addr = inc->i_saddr;
				sin6->sin6_flowinfo = 0;
				sin6->sin6_scope_id = rs->rs_bound_scope_id;
				msg->msg_namelen = sizeof(*sin6);
			}
		}
		break;
	}

	if (inc)
		rds_inc_put(inc);

out:
	return ret;
}
