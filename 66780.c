static int __sys_getsockopt(int fd, int level, int optname,
			    char __user *optval, int __user *optlen)
{
	int err, fput_needed;
	struct socket *sock;

	sock = sockfd_lookup_light(fd, &err, &fput_needed);
	if (sock != NULL) {
		err = security_socket_getsockopt(sock, level, optname);
		if (err)
			goto out_put;

		if (level == SOL_SOCKET)
			err =
			    sock_getsockopt(sock, level, optname, optval,
					    optlen);
		else
			err =
			    sock->ops->getsockopt(sock, level, optname, optval,
						  optlen);
out_put:
		fput_light(sock->file, fput_needed);
	}
	return err;
}
