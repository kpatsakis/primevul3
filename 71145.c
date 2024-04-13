static int hidp_sock_create(struct net *net, struct socket *sock, int protocol,
			    int kern)
{
	struct sock *sk;

	BT_DBG("sock %p", sock);

	if (sock->type != SOCK_RAW)
		return -ESOCKTNOSUPPORT;

	sk = sk_alloc(net, PF_BLUETOOTH, GFP_ATOMIC, &hidp_proto, kern);
	if (!sk)
		return -ENOMEM;

	sock_init_data(sock, sk);

	sock->ops = &hidp_sock_ops;

	sock->state = SS_UNCONNECTED;

	sock_reset_flag(sk, SOCK_ZAPPED);

	sk->sk_protocol = protocol;
	sk->sk_state	= BT_OPEN;

	bt_sock_link(&hidp_sk_list, sk);

	return 0;
}
