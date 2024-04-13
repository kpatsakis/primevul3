static void mac80211_hwsim_beacon_tx(void *arg, u8 *mac,
				     struct ieee80211_vif *vif)
{
	struct mac80211_hwsim_data *data = arg;
	struct ieee80211_hw *hw = data->hw;
	struct ieee80211_tx_info *info;
	struct ieee80211_rate *txrate;
	struct ieee80211_mgmt *mgmt;
	struct sk_buff *skb;

	hwsim_check_magic(vif);

	if (vif->type != NL80211_IFTYPE_AP &&
	    vif->type != NL80211_IFTYPE_MESH_POINT &&
	    vif->type != NL80211_IFTYPE_ADHOC)
		return;

	skb = ieee80211_beacon_get(hw, vif);
	if (skb == NULL)
		return;
	info = IEEE80211_SKB_CB(skb);
	if (ieee80211_hw_check(hw, SUPPORTS_RC_TABLE))
		ieee80211_get_tx_rates(vif, NULL, skb,
				       info->control.rates,
				       ARRAY_SIZE(info->control.rates));

	txrate = ieee80211_get_tx_rate(hw, info);

	mgmt = (struct ieee80211_mgmt *) skb->data;
	/* fake header transmission time */
	data->abs_bcn_ts = mac80211_hwsim_get_tsf_raw();
	mgmt->u.beacon.timestamp = cpu_to_le64(data->abs_bcn_ts +
					       data->tsf_offset +
					       24 * 8 * 10 / txrate->bitrate);

	mac80211_hwsim_tx_frame(hw, skb,
				rcu_dereference(vif->chanctx_conf)->def.chan);

	if (vif->csa_active && ieee80211_csa_is_complete(vif))
		ieee80211_csa_finish(vif);
}
