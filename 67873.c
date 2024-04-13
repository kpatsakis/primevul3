static void mac80211_hwsim_configure_filter(struct ieee80211_hw *hw,
					    unsigned int changed_flags,
					    unsigned int *total_flags,u64 multicast)
{
	struct mac80211_hwsim_data *data = hw->priv;

	wiphy_dbg(hw->wiphy, "%s\n", __func__);

	data->rx_filter = 0;
	if (*total_flags & FIF_ALLMULTI)
		data->rx_filter |= FIF_ALLMULTI;

	*total_flags = data->rx_filter;
}
