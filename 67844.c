static inline int hwsim_net_get_netgroup(struct net *net)
{
	struct hwsim_net *hwsim_net = net_generic(net, hwsim_net_id);

	return hwsim_net->netgroup;
}
