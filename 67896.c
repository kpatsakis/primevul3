static void mac80211_hwsim_stop(struct ieee80211_hw *hw)
{
	struct mac80211_hwsim_data *data = hw->priv;
	data->started = false;
	tasklet_hrtimer_cancel(&data->beacon_timer);
	wiphy_dbg(hw->wiphy, "%s\n", __func__);
}
