static int hwsim_dump_radio_nl(struct sk_buff *skb,
			       struct netlink_callback *cb)
{
	int idx = cb->args[0];
	struct mac80211_hwsim_data *data = NULL;
	int res;

	spin_lock_bh(&hwsim_radio_lock);

	if (idx == hwsim_radio_idx)
		goto done;

	list_for_each_entry(data, &hwsim_radios, list) {
		if (data->idx < idx)
			continue;

		if (!net_eq(wiphy_net(data->hw->wiphy), sock_net(skb->sk)))
			continue;

		res = mac80211_hwsim_get_radio(skb, data,
					       NETLINK_CB(cb->skb).portid,
					       cb->nlh->nlmsg_seq, cb,
					       NLM_F_MULTI);
		if (res < 0)
			break;

		idx = data->idx + 1;
	}

	cb->args[0] = idx;

done:
	spin_unlock_bh(&hwsim_radio_lock);
	return skb->len;
}
