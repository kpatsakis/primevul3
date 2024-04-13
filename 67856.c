static int hwsim_unicast_netgroup(struct mac80211_hwsim_data *data,
				  struct sk_buff *skb, int portid)
{
	struct net *net;
	bool found = false;
	int res = -ENOENT;

	rcu_read_lock();
	for_each_net_rcu(net) {
		if (data->netgroup == hwsim_net_get_netgroup(net)) {
			res = genlmsg_unicast(net, skb, portid);
			found = true;
			break;
		}
	}
	rcu_read_unlock();

	if (!found)
		nlmsg_free(skb);

	return res;
}
