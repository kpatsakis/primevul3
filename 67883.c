static inline u64 mac80211_hwsim_get_tsf_raw(void)
{
	return ktime_to_us(ktime_get_real());
}
