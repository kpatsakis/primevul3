static void __net_exit genl_pernet_exit(struct net *net)
{
	netlink_kernel_release(net->genl_sock);
	net->genl_sock = NULL;
}
