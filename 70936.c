int nfc_llcp_register_device(struct nfc_dev *ndev)
{
	struct nfc_llcp_local *local;

	local = kzalloc(sizeof(struct nfc_llcp_local), GFP_KERNEL);
	if (local == NULL)
		return -ENOMEM;

	local->dev = ndev;
	INIT_LIST_HEAD(&local->list);
	kref_init(&local->ref);
	mutex_init(&local->sdp_lock);
	timer_setup(&local->link_timer, nfc_llcp_symm_timer, 0);

	skb_queue_head_init(&local->tx_queue);
	INIT_WORK(&local->tx_work, nfc_llcp_tx_work);

	local->rx_pending = NULL;
	INIT_WORK(&local->rx_work, nfc_llcp_rx_work);

	INIT_WORK(&local->timeout_work, nfc_llcp_timeout_work);

	rwlock_init(&local->sockets.lock);
	rwlock_init(&local->connecting_sockets.lock);
	rwlock_init(&local->raw_sockets.lock);

	local->lto = 150; /* 1500 ms */
	local->rw = LLCP_MAX_RW;
	local->miux = cpu_to_be16(LLCP_MAX_MIUX);
	local->local_wks = 0x1; /* LLC Link Management */

	nfc_llcp_build_gb(local);

	local->remote_miu = LLCP_DEFAULT_MIU;
	local->remote_lto = LLCP_DEFAULT_LTO;

	mutex_init(&local->sdreq_lock);
	INIT_HLIST_HEAD(&local->pending_sdreqs);
	timer_setup(&local->sdreq_timer, nfc_llcp_sdreq_timer, 0);
	INIT_WORK(&local->sdreq_timeout_work, nfc_llcp_sdreq_timeout_work);

	list_add(&local->list, &llcp_devices);

	return 0;
}
