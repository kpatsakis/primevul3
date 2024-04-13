void *rds_tcp_listen_sock_def_readable(struct net *net)
{
	struct rds_tcp_net *rtn = net_generic(net, rds_tcp_netid);
	struct socket *lsock = rtn->rds_tcp_listen_sock;

	if (!lsock)
		return NULL;

	return lsock->sk->sk_user_data;
}
