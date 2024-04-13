static inline u16 trans_tx_rate_flags_ieee2hwsim(struct ieee80211_tx_rate *rate)
{
	u16 result = 0;

	if (rate->flags & IEEE80211_TX_RC_USE_RTS_CTS)
		result |= MAC80211_HWSIM_TX_RC_USE_RTS_CTS;
	if (rate->flags & IEEE80211_TX_RC_USE_CTS_PROTECT)
		result |= MAC80211_HWSIM_TX_RC_USE_CTS_PROTECT;
	if (rate->flags & IEEE80211_TX_RC_USE_SHORT_PREAMBLE)
		result |= MAC80211_HWSIM_TX_RC_USE_SHORT_PREAMBLE;
	if (rate->flags & IEEE80211_TX_RC_MCS)
		result |= MAC80211_HWSIM_TX_RC_MCS;
	if (rate->flags & IEEE80211_TX_RC_GREEN_FIELD)
		result |= MAC80211_HWSIM_TX_RC_GREEN_FIELD;
	if (rate->flags & IEEE80211_TX_RC_40_MHZ_WIDTH)
		result |= MAC80211_HWSIM_TX_RC_40_MHZ_WIDTH;
	if (rate->flags & IEEE80211_TX_RC_DUP_DATA)
		result |= MAC80211_HWSIM_TX_RC_DUP_DATA;
	if (rate->flags & IEEE80211_TX_RC_SHORT_GI)
		result |= MAC80211_HWSIM_TX_RC_SHORT_GI;
	if (rate->flags & IEEE80211_TX_RC_VHT_MCS)
		result |= MAC80211_HWSIM_TX_RC_VHT_MCS;
	if (rate->flags & IEEE80211_TX_RC_80_MHZ_WIDTH)
		result |= MAC80211_HWSIM_TX_RC_80_MHZ_WIDTH;
	if (rate->flags & IEEE80211_TX_RC_160_MHZ_WIDTH)
		result |= MAC80211_HWSIM_TX_RC_160_MHZ_WIDTH;

	return result;
}
