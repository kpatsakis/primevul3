static int hwsim_register_received_nl(struct sk_buff *skb_2,
				      struct genl_info *info)
{
	struct net *net = genl_info_net(info);
	struct mac80211_hwsim_data *data;
	int chans = 1;

	spin_lock_bh(&hwsim_radio_lock);
	list_for_each_entry(data, &hwsim_radios, list)
		chans = max(chans, data->channels);
	spin_unlock_bh(&hwsim_radio_lock);

	/* In the future we should revise the userspace API and allow it
	 * to set a flag that it does support multi-channel, then we can
	 * let this pass conditionally on the flag.
	 * For current userspace, prohibit it since it won't work right.
	 */
	if (chans > 1)
		return -EOPNOTSUPP;

	if (hwsim_net_get_wmediumd(net))
		return -EBUSY;

	hwsim_register_wmediumd(net, info->snd_portid);

	pr_debug("mac80211_hwsim: received a REGISTER, "
	       "switching to wmediumd mode with pid %d\n", info->snd_portid);

	return 0;
}
