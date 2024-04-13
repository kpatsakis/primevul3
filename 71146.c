static int hidp_sock_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	return do_hidp_sock_ioctl(sock, cmd, (void __user *)arg);
}
