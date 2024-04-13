static void mac80211_hwsim_free(void)
{
	struct mac80211_hwsim_data *data;

	spin_lock_bh(&hwsim_radio_lock);
	while ((data = list_first_entry_or_null(&hwsim_radios,
						struct mac80211_hwsim_data,
						list))) {
		list_del(&data->list);
		spin_unlock_bh(&hwsim_radio_lock);
		mac80211_hwsim_del_radio(data, wiphy_name(data->hw->wiphy),
					 NULL);
		spin_lock_bh(&hwsim_radio_lock);
	}
	spin_unlock_bh(&hwsim_radio_lock);
	class_destroy(hwsim_class);
}
