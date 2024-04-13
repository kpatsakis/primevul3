static void mac80211_hwsim_get_et_strings(struct ieee80211_hw *hw,
					  struct ieee80211_vif *vif,
					  u32 sset, u8 *data)
{
	if (sset == ETH_SS_STATS)
		memcpy(data, *mac80211_hwsim_gstrings_stats,
		       sizeof(mac80211_hwsim_gstrings_stats));
}
