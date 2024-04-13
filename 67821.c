static void hw_scan_work(struct work_struct *work)
{
	struct mac80211_hwsim_data *hwsim =
		container_of(work, struct mac80211_hwsim_data, hw_scan.work);
	struct cfg80211_scan_request *req = hwsim->hw_scan_request;
	int dwell, i;

	mutex_lock(&hwsim->mutex);
	if (hwsim->scan_chan_idx >= req->n_channels) {
		struct cfg80211_scan_info info = {
			.aborted = false,
		};

		wiphy_dbg(hwsim->hw->wiphy, "hw scan complete\n");
		ieee80211_scan_completed(hwsim->hw, &info);
		hwsim->hw_scan_request = NULL;
		hwsim->hw_scan_vif = NULL;
		hwsim->tmp_chan = NULL;
		mutex_unlock(&hwsim->mutex);
		return;
	}

	wiphy_dbg(hwsim->hw->wiphy, "hw scan %d MHz\n",
		  req->channels[hwsim->scan_chan_idx]->center_freq);

	hwsim->tmp_chan = req->channels[hwsim->scan_chan_idx];
	if (hwsim->tmp_chan->flags & (IEEE80211_CHAN_NO_IR |
				      IEEE80211_CHAN_RADAR) ||
	    !req->n_ssids) {
		dwell = 120;
	} else {
		dwell = 30;
		/* send probes */
		for (i = 0; i < req->n_ssids; i++) {
			struct sk_buff *probe;
			struct ieee80211_mgmt *mgmt;

			probe = ieee80211_probereq_get(hwsim->hw,
						       hwsim->scan_addr,
						       req->ssids[i].ssid,
						       req->ssids[i].ssid_len,
						       req->ie_len);
			if (!probe)
				continue;

			mgmt = (struct ieee80211_mgmt *) probe->data;
			memcpy(mgmt->da, req->bssid, ETH_ALEN);
			memcpy(mgmt->bssid, req->bssid, ETH_ALEN);

			if (req->ie_len)
				skb_put_data(probe, req->ie, req->ie_len);

			local_bh_disable();
			mac80211_hwsim_tx_frame(hwsim->hw, probe,
						hwsim->tmp_chan);
			local_bh_enable();
		}
	}
	ieee80211_queue_delayed_work(hwsim->hw, &hwsim->hw_scan,
				     msecs_to_jiffies(dwell));
	hwsim->survey_data[hwsim->scan_chan_idx].channel = hwsim->tmp_chan;
	hwsim->survey_data[hwsim->scan_chan_idx].start = jiffies;
	hwsim->survey_data[hwsim->scan_chan_idx].end =
		jiffies + msecs_to_jiffies(dwell);
	hwsim->scan_chan_idx++;
	mutex_unlock(&hwsim->mutex);
}
