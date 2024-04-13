static int hwsim_fops_ps_write(void *dat, u64 val)
{
	struct mac80211_hwsim_data *data = dat;
	enum ps_mode old_ps;

	if (val != PS_DISABLED && val != PS_ENABLED && val != PS_AUTO_POLL &&
	    val != PS_MANUAL_POLL)
		return -EINVAL;

	if (val == PS_MANUAL_POLL) {
		if (data->ps != PS_ENABLED)
			return -EINVAL;
		local_bh_disable();
		ieee80211_iterate_active_interfaces_atomic(
			data->hw, IEEE80211_IFACE_ITER_NORMAL,
			hwsim_send_ps_poll, data);
		local_bh_enable();
		return 0;
	}
	old_ps = data->ps;
	data->ps = val;

	local_bh_disable();
	if (old_ps == PS_DISABLED && val != PS_DISABLED) {
		ieee80211_iterate_active_interfaces_atomic(
			data->hw, IEEE80211_IFACE_ITER_NORMAL,
			hwsim_send_nullfunc_ps, data);
	} else if (old_ps != PS_DISABLED && val == PS_DISABLED) {
		ieee80211_iterate_active_interfaces_atomic(
			data->hw, IEEE80211_IFACE_ITER_NORMAL,
			hwsim_send_nullfunc_no_ps, data);
	}
	local_bh_enable();

	return 0;
}
