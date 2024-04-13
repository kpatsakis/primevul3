static inline void hwsim_set_sta_magic(struct ieee80211_sta *sta)
{
	struct hwsim_sta_priv *sp = (void *)sta->drv_priv;
	sp->magic = HWSIM_STA_MAGIC;
}
