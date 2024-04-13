int kernel_getpeername(struct socket *sock, struct sockaddr *addr)
{
	return sock->ops->getname(sock, addr, 1);
}
