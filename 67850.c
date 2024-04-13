static void hwsim_register_wmediumd(struct net *net, u32 portid)
{
	struct mac80211_hwsim_data *data;

	hwsim_net_set_wmediumd(net, portid);

	spin_lock_bh(&hwsim_radio_lock);
	list_for_each_entry(data, &hwsim_radios, list) {
		if (data->netgroup == hwsim_net_get_netgroup(net))
			data->wmediumd = portid;
	}
	spin_unlock_bh(&hwsim_radio_lock);
}
