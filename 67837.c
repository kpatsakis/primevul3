static int hwsim_get_radio_nl(struct sk_buff *msg, struct genl_info *info)
{
	struct mac80211_hwsim_data *data;
	struct sk_buff *skb;
	int idx, res = -ENODEV;

	if (!info->attrs[HWSIM_ATTR_RADIO_ID])
		return -EINVAL;
	idx = nla_get_u32(info->attrs[HWSIM_ATTR_RADIO_ID]);

	spin_lock_bh(&hwsim_radio_lock);
	list_for_each_entry(data, &hwsim_radios, list) {
		if (data->idx != idx)
			continue;

		if (!net_eq(wiphy_net(data->hw->wiphy), genl_info_net(info)))
			continue;

		skb = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_ATOMIC);
		if (!skb) {
			res = -ENOMEM;
			goto out_err;
		}

		res = mac80211_hwsim_get_radio(skb, data, info->snd_portid,
					       info->snd_seq, NULL, 0);
		if (res < 0) {
			nlmsg_free(skb);
			goto out_err;
		}

		genlmsg_reply(skb, info);
		break;
	}

out_err:
	spin_unlock_bh(&hwsim_radio_lock);

	return res;
}
