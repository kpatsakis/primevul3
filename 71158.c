void rds_tcp_nonagle(struct socket *sock)
{
	int val = 1;

	kernel_setsockopt(sock, SOL_TCP, TCP_NODELAY, (void *)&val,
			      sizeof(val));
}
