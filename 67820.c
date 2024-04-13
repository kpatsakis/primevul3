static void hw_roc_start(struct work_struct *work)
{
	struct mac80211_hwsim_data *hwsim =
		container_of(work, struct mac80211_hwsim_data, roc_start.work);

	mutex_lock(&hwsim->mutex);

	wiphy_dbg(hwsim->hw->wiphy, "hwsim ROC begins\n");
	hwsim->tmp_chan = hwsim->roc_chan;
	ieee80211_ready_on_channel(hwsim->hw);

	ieee80211_queue_delayed_work(hwsim->hw, &hwsim->roc_done,
				     msecs_to_jiffies(hwsim->roc_duration));

	mutex_unlock(&hwsim->mutex);
}
