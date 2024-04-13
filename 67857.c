static int hwsim_write_simulate_radar(void *dat, u64 val)
{
	struct mac80211_hwsim_data *data = dat;

	ieee80211_radar_detected(data->hw);

	return 0;
}
