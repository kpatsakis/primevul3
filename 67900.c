static void mac80211_hwsim_tx_iter(void *_data, u8 *addr,
				   struct ieee80211_vif *vif)
{
	struct tx_iter_data *data = _data;

	if (!vif->chanctx_conf)
		return;

	if (!hwsim_chans_compat(data->channel,
				rcu_dereference(vif->chanctx_conf)->def.chan))
		return;

	data->receive = true;
}
