static inline void hwsim_check_sta_magic(struct ieee80211_sta *sta)
{
	struct hwsim_sta_priv *sp = (void *)sta->drv_priv;
	WARN_ON(sp->magic != HWSIM_STA_MAGIC);
}
