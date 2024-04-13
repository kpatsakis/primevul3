static void mac80211_hwsim_bcn_en_iter(void *data, u8 *mac,
				       struct ieee80211_vif *vif)
{
	unsigned int *count = data;
	struct hwsim_vif_priv *vp = (void *)vif->drv_priv;

	if (vp->bcn_en)
		(*count)++;
}
