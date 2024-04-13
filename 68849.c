 static void hns_nic_drop_rx_fetch(struct hns_nic_ring_data *ring_data,
				  struct sk_buff *skb)
{
	dev_kfree_skb_any(skb);
}
