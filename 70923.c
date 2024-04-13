void nfc_llcp_mac_is_up(struct nfc_dev *dev, u32 target_idx,
			u8 comm_mode, u8 rf_mode)
{
	struct nfc_llcp_local *local;

	pr_debug("rf mode %d\n", rf_mode);

	local = nfc_llcp_find_local(dev);
	if (local == NULL)
		return;

	local->target_idx = target_idx;
	local->comm_mode = comm_mode;
	local->rf_mode = rf_mode;

	if (rf_mode == NFC_RF_INITIATOR) {
		pr_debug("Queueing Tx work\n");

		schedule_work(&local->tx_work);
	} else {
		mod_timer(&local->link_timer,
			  jiffies + msecs_to_jiffies(local->remote_lto));
	}
}
