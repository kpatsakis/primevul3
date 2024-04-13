static int mac80211_hwsim_new_radio(struct genl_info *info,
				    struct hwsim_new_radio_params *param)
{
	int err;
	u8 addr[ETH_ALEN];
	struct mac80211_hwsim_data *data;
	struct ieee80211_hw *hw;
	enum nl80211_band band;
	const struct ieee80211_ops *ops = &mac80211_hwsim_ops;
	struct net *net;
	int idx;

	if (WARN_ON(param->channels > 1 && !param->use_chanctx))
		return -EINVAL;

	spin_lock_bh(&hwsim_radio_lock);
	idx = hwsim_radio_idx++;
	spin_unlock_bh(&hwsim_radio_lock);

	if (param->use_chanctx)
		ops = &mac80211_hwsim_mchan_ops;
	hw = ieee80211_alloc_hw_nm(sizeof(*data), ops, param->hwname);
	if (!hw) {
		pr_debug("mac80211_hwsim: ieee80211_alloc_hw failed\n");
		err = -ENOMEM;
		goto failed;
	}

	/* ieee80211_alloc_hw_nm may have used a default name */
	param->hwname = wiphy_name(hw->wiphy);

	if (info)
		net = genl_info_net(info);
	else
		net = &init_net;
	wiphy_net_set(hw->wiphy, net);

	data = hw->priv;
	data->hw = hw;

	data->dev = device_create(hwsim_class, NULL, 0, hw, "hwsim%d", idx);
	if (IS_ERR(data->dev)) {
		printk(KERN_DEBUG
		       "mac80211_hwsim: device_create failed (%ld)\n",
		       PTR_ERR(data->dev));
		err = -ENOMEM;
		goto failed_drvdata;
	}
	data->dev->driver = &mac80211_hwsim_driver.driver;
	err = device_bind_driver(data->dev);
	if (err != 0) {
		pr_debug("mac80211_hwsim: device_bind_driver failed (%d)\n",
		       err);
		goto failed_bind;
	}

	skb_queue_head_init(&data->pending);

	SET_IEEE80211_DEV(hw, data->dev);
	eth_zero_addr(addr);
	addr[0] = 0x02;
	addr[3] = idx >> 8;
	addr[4] = idx;
	memcpy(data->addresses[0].addr, addr, ETH_ALEN);
	memcpy(data->addresses[1].addr, addr, ETH_ALEN);
	data->addresses[1].addr[0] |= 0x40;
	hw->wiphy->n_addresses = 2;
	hw->wiphy->addresses = data->addresses;

	data->channels = param->channels;
	data->use_chanctx = param->use_chanctx;
	data->idx = idx;
	data->destroy_on_close = param->destroy_on_close;
	if (info)
		data->portid = info->snd_portid;

	if (data->use_chanctx) {
		hw->wiphy->max_scan_ssids = 255;
		hw->wiphy->max_scan_ie_len = IEEE80211_MAX_DATA_LEN;
		hw->wiphy->max_remain_on_channel_duration = 1000;
		hw->wiphy->iface_combinations = &data->if_combination;
		if (param->p2p_device)
			data->if_combination = hwsim_if_comb_p2p_dev[0];
		else
			data->if_combination = hwsim_if_comb[0];
		hw->wiphy->n_iface_combinations = 1;
		/* For channels > 1 DFS is not allowed */
		data->if_combination.radar_detect_widths = 0;
		data->if_combination.num_different_channels = data->channels;
	} else if (param->p2p_device) {
		hw->wiphy->iface_combinations = hwsim_if_comb_p2p_dev;
		hw->wiphy->n_iface_combinations =
			ARRAY_SIZE(hwsim_if_comb_p2p_dev);
	} else {
		hw->wiphy->iface_combinations = hwsim_if_comb;
		hw->wiphy->n_iface_combinations = ARRAY_SIZE(hwsim_if_comb);
	}

	INIT_DELAYED_WORK(&data->roc_start, hw_roc_start);
	INIT_DELAYED_WORK(&data->roc_done, hw_roc_done);
	INIT_DELAYED_WORK(&data->hw_scan, hw_scan_work);

	hw->queues = 5;
	hw->offchannel_tx_hw_queue = 4;
	hw->wiphy->interface_modes = BIT(NL80211_IFTYPE_STATION) |
				     BIT(NL80211_IFTYPE_AP) |
				     BIT(NL80211_IFTYPE_P2P_CLIENT) |
				     BIT(NL80211_IFTYPE_P2P_GO) |
				     BIT(NL80211_IFTYPE_ADHOC) |
				     BIT(NL80211_IFTYPE_MESH_POINT);

	if (param->p2p_device)
		hw->wiphy->interface_modes |= BIT(NL80211_IFTYPE_P2P_DEVICE);

	ieee80211_hw_set(hw, SUPPORT_FAST_XMIT);
	ieee80211_hw_set(hw, CHANCTX_STA_CSA);
	ieee80211_hw_set(hw, SUPPORTS_HT_CCK_RATES);
	ieee80211_hw_set(hw, QUEUE_CONTROL);
	ieee80211_hw_set(hw, WANT_MONITOR_VIF);
	ieee80211_hw_set(hw, AMPDU_AGGREGATION);
	ieee80211_hw_set(hw, MFP_CAPABLE);
	ieee80211_hw_set(hw, SIGNAL_DBM);
	ieee80211_hw_set(hw, TDLS_WIDER_BW);
	if (rctbl)
		ieee80211_hw_set(hw, SUPPORTS_RC_TABLE);

	hw->wiphy->flags |= WIPHY_FLAG_SUPPORTS_TDLS |
			    WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL |
			    WIPHY_FLAG_AP_UAPSD |
			    WIPHY_FLAG_HAS_CHANNEL_SWITCH;
	hw->wiphy->features |= NL80211_FEATURE_ACTIVE_MONITOR |
			       NL80211_FEATURE_AP_MODE_CHAN_WIDTH_CHANGE |
			       NL80211_FEATURE_STATIC_SMPS |
			       NL80211_FEATURE_DYNAMIC_SMPS |
			       NL80211_FEATURE_SCAN_RANDOM_MAC_ADDR;
	wiphy_ext_feature_set(hw->wiphy, NL80211_EXT_FEATURE_VHT_IBSS);

	/* ask mac80211 to reserve space for magic */
	hw->vif_data_size = sizeof(struct hwsim_vif_priv);
	hw->sta_data_size = sizeof(struct hwsim_sta_priv);
	hw->chanctx_data_size = sizeof(struct hwsim_chanctx_priv);

	memcpy(data->channels_2ghz, hwsim_channels_2ghz,
		sizeof(hwsim_channels_2ghz));
	memcpy(data->channels_5ghz, hwsim_channels_5ghz,
		sizeof(hwsim_channels_5ghz));
	memcpy(data->rates, hwsim_rates, sizeof(hwsim_rates));

	for (band = NL80211_BAND_2GHZ; band < NUM_NL80211_BANDS; band++) {
		struct ieee80211_supported_band *sband = &data->bands[band];
		switch (band) {
		case NL80211_BAND_2GHZ:
			sband->channels = data->channels_2ghz;
			sband->n_channels = ARRAY_SIZE(hwsim_channels_2ghz);
			sband->bitrates = data->rates;
			sband->n_bitrates = ARRAY_SIZE(hwsim_rates);
			break;
		case NL80211_BAND_5GHZ:
			sband->channels = data->channels_5ghz;
			sband->n_channels = ARRAY_SIZE(hwsim_channels_5ghz);
			sband->bitrates = data->rates + 4;
			sband->n_bitrates = ARRAY_SIZE(hwsim_rates) - 4;

			sband->vht_cap.vht_supported = true;
			sband->vht_cap.cap =
				IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_11454 |
				IEEE80211_VHT_CAP_SUPP_CHAN_WIDTH_160_80PLUS80MHZ |
				IEEE80211_VHT_CAP_RXLDPC |
				IEEE80211_VHT_CAP_SHORT_GI_80 |
				IEEE80211_VHT_CAP_SHORT_GI_160 |
				IEEE80211_VHT_CAP_TXSTBC |
				IEEE80211_VHT_CAP_RXSTBC_1 |
				IEEE80211_VHT_CAP_RXSTBC_2 |
				IEEE80211_VHT_CAP_RXSTBC_3 |
				IEEE80211_VHT_CAP_RXSTBC_4 |
				IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_MASK;
			sband->vht_cap.vht_mcs.rx_mcs_map =
				cpu_to_le16(IEEE80211_VHT_MCS_SUPPORT_0_9 << 0 |
					    IEEE80211_VHT_MCS_SUPPORT_0_9 << 2 |
					    IEEE80211_VHT_MCS_SUPPORT_0_9 << 4 |
					    IEEE80211_VHT_MCS_SUPPORT_0_9 << 6 |
					    IEEE80211_VHT_MCS_SUPPORT_0_9 << 8 |
					    IEEE80211_VHT_MCS_SUPPORT_0_9 << 10 |
					    IEEE80211_VHT_MCS_SUPPORT_0_9 << 12 |
					    IEEE80211_VHT_MCS_SUPPORT_0_9 << 14);
			sband->vht_cap.vht_mcs.tx_mcs_map =
				sband->vht_cap.vht_mcs.rx_mcs_map;
			break;
		default:
			continue;
		}

		sband->ht_cap.ht_supported = true;
		sband->ht_cap.cap = IEEE80211_HT_CAP_SUP_WIDTH_20_40 |
				    IEEE80211_HT_CAP_GRN_FLD |
				    IEEE80211_HT_CAP_SGI_20 |
				    IEEE80211_HT_CAP_SGI_40 |
				    IEEE80211_HT_CAP_DSSSCCK40;
		sband->ht_cap.ampdu_factor = 0x3;
		sband->ht_cap.ampdu_density = 0x6;
		memset(&sband->ht_cap.mcs, 0,
		       sizeof(sband->ht_cap.mcs));
		sband->ht_cap.mcs.rx_mask[0] = 0xff;
		sband->ht_cap.mcs.rx_mask[1] = 0xff;
		sband->ht_cap.mcs.tx_params = IEEE80211_HT_MCS_TX_DEFINED;

		hw->wiphy->bands[band] = sband;
	}

	/* By default all radios belong to the first group */
	data->group = 1;
	mutex_init(&data->mutex);

	data->netgroup = hwsim_net_get_netgroup(net);

	/* Enable frame retransmissions for lossy channels */
	hw->max_rates = 4;
	hw->max_rate_tries = 11;

	hw->wiphy->vendor_commands = mac80211_hwsim_vendor_commands;
	hw->wiphy->n_vendor_commands =
		ARRAY_SIZE(mac80211_hwsim_vendor_commands);
	hw->wiphy->vendor_events = mac80211_hwsim_vendor_events;
	hw->wiphy->n_vendor_events = ARRAY_SIZE(mac80211_hwsim_vendor_events);

	if (param->reg_strict)
		hw->wiphy->regulatory_flags |= REGULATORY_STRICT_REG;
	if (param->regd) {
		data->regd = param->regd;
		hw->wiphy->regulatory_flags |= REGULATORY_CUSTOM_REG;
		wiphy_apply_custom_regulatory(hw->wiphy, param->regd);
		/* give the regulatory workqueue a chance to run */
		schedule_timeout_interruptible(1);
	}

	if (param->no_vif)
		ieee80211_hw_set(hw, NO_AUTO_VIF);

	wiphy_ext_feature_set(hw->wiphy, NL80211_EXT_FEATURE_CQM_RSSI_LIST);

	err = ieee80211_register_hw(hw);
	if (err < 0) {
		pr_debug("mac80211_hwsim: ieee80211_register_hw failed (%d)\n",
		       err);
		goto failed_hw;
	}

	wiphy_dbg(hw->wiphy, "hwaddr %pM registered\n", hw->wiphy->perm_addr);

	if (param->reg_alpha2) {
		data->alpha2[0] = param->reg_alpha2[0];
		data->alpha2[1] = param->reg_alpha2[1];
		regulatory_hint(hw->wiphy, param->reg_alpha2);
	}

	data->debugfs = debugfs_create_dir("hwsim", hw->wiphy->debugfsdir);
	debugfs_create_file("ps", 0666, data->debugfs, data, &hwsim_fops_ps);
	debugfs_create_file("group", 0666, data->debugfs, data,
			    &hwsim_fops_group);
	if (!data->use_chanctx)
		debugfs_create_file("dfs_simulate_radar", 0222,
				    data->debugfs,
				    data, &hwsim_simulate_radar);

	tasklet_hrtimer_init(&data->beacon_timer,
			     mac80211_hwsim_beacon,
			     CLOCK_MONOTONIC, HRTIMER_MODE_ABS);

	spin_lock_bh(&hwsim_radio_lock);
	err = rhashtable_insert_fast(&hwsim_radios_rht, &data->rht,
				     hwsim_rht_params);
	if (err < 0) {
		pr_debug("mac80211_hwsim: radio index %d already present\n",
			 idx);
		spin_unlock_bh(&hwsim_radio_lock);
		goto failed_final_insert;
	}

	list_add_tail(&data->list, &hwsim_radios);
	spin_unlock_bh(&hwsim_radio_lock);

	if (idx > 0)
		hwsim_mcast_new_radio(idx, info, param);

	return idx;

failed_final_insert:
	debugfs_remove_recursive(data->debugfs);
	ieee80211_unregister_hw(data->hw);
failed_hw:
	device_release_driver(data->dev);
failed_bind:
	device_unregister(data->dev);
failed_drvdata:
	ieee80211_free_hw(hw);
failed:
	return err;
}
