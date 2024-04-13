mac80211_hwsim_beacon(struct hrtimer *timer)
{
	struct mac80211_hwsim_data *data =
		container_of(timer, struct mac80211_hwsim_data,
			     beacon_timer.timer);
	struct ieee80211_hw *hw = data->hw;
	u64 bcn_int = data->beacon_int;
	ktime_t next_bcn;

	if (!data->started)
		goto out;

	ieee80211_iterate_active_interfaces_atomic(
		hw, IEEE80211_IFACE_ITER_NORMAL,
		mac80211_hwsim_beacon_tx, data);

	/* beacon at new TBTT + beacon interval */
	if (data->bcn_delta) {
		bcn_int -= data->bcn_delta;
		data->bcn_delta = 0;
	}

	next_bcn = ktime_add(hrtimer_get_expires(timer),
			     ns_to_ktime(bcn_int * 1000));
	tasklet_hrtimer_start(&data->beacon_timer, next_bcn, HRTIMER_MODE_ABS);
out:
	return HRTIMER_NORESTART;
}
