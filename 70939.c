static void nfc_llcp_rx_work(struct work_struct *work)
{
	struct nfc_llcp_local *local = container_of(work, struct nfc_llcp_local,
						    rx_work);
	struct sk_buff *skb;

	skb = local->rx_pending;
	if (skb == NULL) {
		pr_debug("No pending SKB\n");
		return;
	}

	__net_timestamp(skb);

	nfc_llcp_send_to_raw_sock(local, skb, NFC_DIRECTION_RX);

	nfc_llcp_rx_skb(local, skb);

	schedule_work(&local->tx_work);
	kfree_skb(local->rx_pending);
	local->rx_pending = NULL;
}
