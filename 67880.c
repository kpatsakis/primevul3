static int mac80211_hwsim_get_radio(struct sk_buff *skb,
				    struct mac80211_hwsim_data *data,
				    u32 portid, u32 seq,
				    struct netlink_callback *cb, int flags)
{
	void *hdr;
	struct hwsim_new_radio_params param = { };
	int res = -EMSGSIZE;

	hdr = genlmsg_put(skb, portid, seq, &hwsim_genl_family, flags,
			  HWSIM_CMD_GET_RADIO);
	if (!hdr)
		return -EMSGSIZE;

	if (cb)
		genl_dump_check_consistent(cb, hdr);

	if (data->alpha2[0] && data->alpha2[1])
		param.reg_alpha2 = data->alpha2;

	param.reg_strict = !!(data->hw->wiphy->regulatory_flags &
					REGULATORY_STRICT_REG);
	param.p2p_device = !!(data->hw->wiphy->interface_modes &
					BIT(NL80211_IFTYPE_P2P_DEVICE));
	param.use_chanctx = data->use_chanctx;
	param.regd = data->regd;
	param.channels = data->channels;
	param.hwname = wiphy_name(data->hw->wiphy);

	res = append_radio_msg(skb, data->idx, &param);
	if (res < 0)
		goto out_err;

	genlmsg_end(skb, hdr);
	return 0;

out_err:
	genlmsg_cancel(skb, hdr);
	return res;
}
