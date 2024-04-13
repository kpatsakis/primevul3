int __sys_getsockname(int fd, struct sockaddr __user *usockaddr,
		      int __user *usockaddr_len)
{
	struct socket *sock;
	struct sockaddr_storage address;
	int err, fput_needed;

	sock = sockfd_lookup_light(fd, &err, &fput_needed);
	if (!sock)
		goto out;

	err = security_socket_getsockname(sock);
	if (err)
		goto out_put;

	err = sock->ops->getname(sock, (struct sockaddr *)&address, 0);
	if (err < 0)
		goto out_put;
        /* "err" is actually length in this case */
	err = move_addr_to_user(&address, err, usockaddr, usockaddr_len);

out_put:
	fput_light(sock->file, fput_needed);
out:
	return err;
}
