static int l2tp_ip6_disconnect(struct sock *sk, int flags)
{
	if (sock_flag(sk, SOCK_ZAPPED))
		return 0;

	return __udp_disconnect(sk, flags);
}
