static struct mac80211_hwsim_data *get_hwsim_data_ref_from_addr(const u8 *addr)
{
	return rhashtable_lookup_fast(&hwsim_radios_rht,
				      addr,
				      hwsim_rht_params);
}
