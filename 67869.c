static void mac80211_hwsim_bss_info_changed(struct ieee80211_hw *hw,
					    struct ieee80211_vif *vif,
					    struct ieee80211_bss_conf *info,
					    u32 changed)
{
	struct hwsim_vif_priv *vp = (void *)vif->drv_priv;
	struct mac80211_hwsim_data *data = hw->priv;

	hwsim_check_magic(vif);

	wiphy_dbg(hw->wiphy, "%s(changed=0x%x vif->addr=%pM)\n",
		  __func__, changed, vif->addr);

	if (changed & BSS_CHANGED_BSSID) {
		wiphy_dbg(hw->wiphy, "%s: BSSID changed: %pM\n",
			  __func__, info->bssid);
		memcpy(vp->bssid, info->bssid, ETH_ALEN);
	}

	if (changed & BSS_CHANGED_ASSOC) {
		wiphy_dbg(hw->wiphy, "  ASSOC: assoc=%d aid=%d\n",
			  info->assoc, info->aid);
		vp->assoc = info->assoc;
		vp->aid = info->aid;
	}

	if (changed & BSS_CHANGED_BEACON_ENABLED) {
		wiphy_dbg(hw->wiphy, "  BCN EN: %d (BI=%u)\n",
			  info->enable_beacon, info->beacon_int);
		vp->bcn_en = info->enable_beacon;
		if (data->started &&
		    !hrtimer_is_queued(&data->beacon_timer.timer) &&
		    info->enable_beacon) {
			u64 tsf, until_tbtt;
			u32 bcn_int;
			data->beacon_int = info->beacon_int * 1024;
			tsf = mac80211_hwsim_get_tsf(hw, vif);
			bcn_int = data->beacon_int;
			until_tbtt = bcn_int - do_div(tsf, bcn_int);
			tasklet_hrtimer_start(&data->beacon_timer,
					      ns_to_ktime(until_tbtt * 1000),
					      HRTIMER_MODE_REL);
		} else if (!info->enable_beacon) {
			unsigned int count = 0;
			ieee80211_iterate_active_interfaces_atomic(
				data->hw, IEEE80211_IFACE_ITER_NORMAL,
				mac80211_hwsim_bcn_en_iter, &count);
			wiphy_dbg(hw->wiphy, "  beaconing vifs remaining: %u",
				  count);
			if (count == 0) {
				tasklet_hrtimer_cancel(&data->beacon_timer);
				data->beacon_int = 0;
			}
		}
	}

	if (changed & BSS_CHANGED_ERP_CTS_PROT) {
		wiphy_dbg(hw->wiphy, "  ERP_CTS_PROT: %d\n",
			  info->use_cts_prot);
	}

	if (changed & BSS_CHANGED_ERP_PREAMBLE) {
		wiphy_dbg(hw->wiphy, "  ERP_PREAMBLE: %d\n",
			  info->use_short_preamble);
	}

	if (changed & BSS_CHANGED_ERP_SLOT) {
		wiphy_dbg(hw->wiphy, "  ERP_SLOT: %d\n", info->use_short_slot);
	}

	if (changed & BSS_CHANGED_HT) {
		wiphy_dbg(hw->wiphy, "  HT: op_mode=0x%x\n",
			  info->ht_operation_mode);
	}

	if (changed & BSS_CHANGED_BASIC_RATES) {
		wiphy_dbg(hw->wiphy, "  BASIC_RATES: 0x%llx\n",
			  (unsigned long long) info->basic_rates);
	}

	if (changed & BSS_CHANGED_TXPOWER)
		wiphy_dbg(hw->wiphy, "  TX Power: %d dBm\n", info->txpower);
}
