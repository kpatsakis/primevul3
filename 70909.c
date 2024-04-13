static void __nfc_llcp_recv(struct nfc_llcp_local *local, struct sk_buff *skb)
{
	local->rx_pending = skb;
	del_timer(&local->link_timer);
	schedule_work(&local->rx_work);
}
