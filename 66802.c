int kernel_getsockname(struct socket *sock, struct sockaddr *addr)
{
	return sock->ops->getname(sock, addr, 0);
}
