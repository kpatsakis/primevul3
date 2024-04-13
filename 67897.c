static void mac80211_hwsim_sw_scan(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif,
				   const u8 *mac_addr)
{
	struct mac80211_hwsim_data *hwsim = hw->priv;

	mutex_lock(&hwsim->mutex);

	if (hwsim->scanning) {
		pr_debug("two hwsim sw_scans detected!\n");
		goto out;
	}

	pr_debug("hwsim sw_scan request, prepping stuff\n");

	memcpy(hwsim->scan_addr, mac_addr, ETH_ALEN);
	hwsim->scanning = true;
	memset(hwsim->survey_data, 0, sizeof(hwsim->survey_data));

out:
	mutex_unlock(&hwsim->mutex);
}
