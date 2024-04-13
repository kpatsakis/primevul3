static void mac80211_hwsim_addr_iter(void *data, u8 *mac,
				     struct ieee80211_vif *vif)
{
	struct mac80211_hwsim_addr_match_data *md = data;

	if (memcmp(mac, md->addr, ETH_ALEN) == 0)
		md->ret = true;
}
