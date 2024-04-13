static int mac80211_hwsim_start(struct ieee80211_hw *hw)
{
	struct mac80211_hwsim_data *data = hw->priv;
	wiphy_dbg(hw->wiphy, "%s\n", __func__);
	data->started = true;
	return 0;
}
