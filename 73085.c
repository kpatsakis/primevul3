static int tun_sendmsg(struct socket *sock, struct msghdr *m, size_t total_len)
{
	int ret;
	struct tun_file *tfile = container_of(sock, struct tun_file, socket);
	struct tun_struct *tun = __tun_get(tfile);

	if (!tun)
		return -EBADFD;

	ret = tun_get_user(tun, tfile, m->msg_control, &m->msg_iter,
			   m->msg_flags & MSG_DONTWAIT,
			   m->msg_flags & MSG_MORE);
	tun_put(tun);
	return ret;
}
