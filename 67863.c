static bool mac80211_hwsim_addr_match(struct mac80211_hwsim_data *data,
				      const u8 *addr)
{
	struct mac80211_hwsim_addr_match_data md = {
		.ret = false,
	};

	if (data->scanning && memcmp(addr, data->scan_addr, ETH_ALEN) == 0)
		return true;

	memcpy(md.addr, addr, ETH_ALEN);

	ieee80211_iterate_active_interfaces_atomic(data->hw,
						   IEEE80211_IFACE_ITER_NORMAL,
						   mac80211_hwsim_addr_iter,
						   &md);

	return md.ret;
}
