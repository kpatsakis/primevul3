static void local_cleanup(struct nfc_llcp_local *local)
{
	nfc_llcp_socket_release(local, false, ENXIO);
	del_timer_sync(&local->link_timer);
	skb_queue_purge(&local->tx_queue);
	cancel_work_sync(&local->tx_work);
	cancel_work_sync(&local->rx_work);
	cancel_work_sync(&local->timeout_work);
	kfree_skb(local->rx_pending);
	del_timer_sync(&local->sdreq_timer);
	cancel_work_sync(&local->sdreq_timeout_work);
	nfc_llcp_free_sdp_tlv_list(&local->pending_sdreqs);
}
