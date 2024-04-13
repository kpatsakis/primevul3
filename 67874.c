static void mac80211_hwsim_del_radio(struct mac80211_hwsim_data *data,
				     const char *hwname,
				     struct genl_info *info)
{
	hwsim_mcast_del_radio(data->idx, hwname, info);
	debugfs_remove_recursive(data->debugfs);
	ieee80211_unregister_hw(data->hw);
	device_release_driver(data->dev);
	device_unregister(data->dev);
	ieee80211_free_hw(data->hw);
}
