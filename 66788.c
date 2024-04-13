static int __sys_setsockopt(int fd, int level, int optname,
			    char __user *optval, int optlen)
{
	int err, fput_needed;
	struct socket *sock;

	if (optlen < 0)
		return -EINVAL;

	sock = sockfd_lookup_light(fd, &err, &fput_needed);
	if (sock != NULL) {
		err = security_socket_setsockopt(sock, level, optname);
		if (err)
			goto out_put;

		if (level == SOL_SOCKET)
			err =
			    sock_setsockopt(sock, level, optname, optval,
					    optlen);
		else
			err =
			    sock->ops->setsockopt(sock, level, optname, optval,
						  optlen);
out_put:
		fput_light(sock->file, fput_needed);
	}
	return err;
}
