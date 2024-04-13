static int hwsim_del_radio_nl(struct sk_buff *msg, struct genl_info *info)
{
	struct mac80211_hwsim_data *data;
	s64 idx = -1;
	const char *hwname = NULL;

	if (info->attrs[HWSIM_ATTR_RADIO_ID]) {
		idx = nla_get_u32(info->attrs[HWSIM_ATTR_RADIO_ID]);
	} else if (info->attrs[HWSIM_ATTR_RADIO_NAME]) {
		hwname = kasprintf(GFP_KERNEL, "%.*s",
				   nla_len(info->attrs[HWSIM_ATTR_RADIO_NAME]),
				   (char *)nla_data(info->attrs[HWSIM_ATTR_RADIO_NAME]));
		if (!hwname)
			return -ENOMEM;
	} else
		return -EINVAL;

	spin_lock_bh(&hwsim_radio_lock);
	list_for_each_entry(data, &hwsim_radios, list) {
		if (idx >= 0) {
			if (data->idx != idx)
				continue;
		} else {
			if (!hwname ||
			    strcmp(hwname, wiphy_name(data->hw->wiphy)))
				continue;
		}

		if (!net_eq(wiphy_net(data->hw->wiphy), genl_info_net(info)))
			continue;

		list_del(&data->list);
		rhashtable_remove_fast(&hwsim_radios_rht, &data->rht,
				       hwsim_rht_params);
		spin_unlock_bh(&hwsim_radio_lock);
		mac80211_hwsim_del_radio(data, wiphy_name(data->hw->wiphy),
					 info);
		kfree(hwname);
		return 0;
	}
	spin_unlock_bh(&hwsim_radio_lock);

	kfree(hwname);
	return -ENODEV;
}
