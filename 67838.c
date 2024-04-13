static __net_init int hwsim_init_net(struct net *net)
{
	hwsim_net_set_netgroup(net);

	return 0;
}
