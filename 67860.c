static int mac80211_hwsim_add_interface(struct ieee80211_hw *hw,
					struct ieee80211_vif *vif)
{
	wiphy_dbg(hw->wiphy, "%s (type=%d mac_addr=%pM)\n",
		  __func__, ieee80211_vif_type_p2p(vif),
		  vif->addr);
	hwsim_set_magic(vif);

	vif->cab_queue = 0;
	vif->hw_queue[IEEE80211_AC_VO] = 0;
	vif->hw_queue[IEEE80211_AC_VI] = 1;
	vif->hw_queue[IEEE80211_AC_BE] = 2;
	vif->hw_queue[IEEE80211_AC_BK] = 3;

	return 0;
}
