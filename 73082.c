static int tun_recvmsg(struct socket *sock, struct msghdr *m, size_t total_len,
		       int flags)
{
	struct tun_file *tfile = container_of(sock, struct tun_file, socket);
	struct tun_struct *tun = __tun_get(tfile);
	int ret;

	if (!tun)
		return -EBADFD;

	if (flags & ~(MSG_DONTWAIT|MSG_TRUNC|MSG_ERRQUEUE)) {
		ret = -EINVAL;
		goto out;
	}
	if (flags & MSG_ERRQUEUE) {
		ret = sock_recv_errqueue(sock->sk, m, total_len,
					 SOL_PACKET, TUN_TX_TIMESTAMP);
		goto out;
	}
	ret = tun_do_read(tun, tfile, &m->msg_iter, flags & MSG_DONTWAIT,
			  m->msg_control);
	if (ret > (ssize_t)total_len) {
		m->msg_flags |= MSG_TRUNC;
		ret = flags & MSG_TRUNC ? ret : total_len;
	}
out:
	tun_put(tun);
	return ret;
}
