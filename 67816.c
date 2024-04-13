static void destroy_radio(struct work_struct *work)
{
	struct mac80211_hwsim_data *data =
		container_of(work, struct mac80211_hwsim_data, destroy_work);

	mac80211_hwsim_del_radio(data, wiphy_name(data->hw->wiphy), NULL);
}
