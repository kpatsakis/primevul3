static void hw_roc_done(struct work_struct *work)
{
	struct mac80211_hwsim_data *hwsim =
		container_of(work, struct mac80211_hwsim_data, roc_done.work);

	mutex_lock(&hwsim->mutex);
	ieee80211_remain_on_channel_expired(hwsim->hw);
	hwsim->tmp_chan = NULL;
	mutex_unlock(&hwsim->mutex);

	wiphy_dbg(hwsim->hw->wiphy, "hwsim ROC expired\n");
}
