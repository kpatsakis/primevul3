static void remove_user_radios(u32 portid)
{
	struct mac80211_hwsim_data *entry, *tmp;

	spin_lock_bh(&hwsim_radio_lock);
	list_for_each_entry_safe(entry, tmp, &hwsim_radios, list) {
		if (entry->destroy_on_close && entry->portid == portid) {
			list_del(&entry->list);
			rhashtable_remove_fast(&hwsim_radios_rht, &entry->rht,
					       hwsim_rht_params);
			INIT_WORK(&entry->destroy_work, destroy_radio);
			schedule_work(&entry->destroy_work);
		}
	}
	spin_unlock_bh(&hwsim_radio_lock);
}
