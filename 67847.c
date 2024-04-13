static inline void hwsim_net_set_wmediumd(struct net *net, u32 portid)
{
	struct hwsim_net *hwsim_net = net_generic(net, hwsim_net_id);

	hwsim_net->wmediumd = portid;
}
