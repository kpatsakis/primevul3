static u64 mac80211_hwsim_get_tsf(struct ieee80211_hw *hw,
				  struct ieee80211_vif *vif)
{
	struct mac80211_hwsim_data *data = hw->priv;
	return le64_to_cpu(__mac80211_hwsim_get_tsf(data));
}
