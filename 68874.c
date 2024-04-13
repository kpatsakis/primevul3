static void hns_nic_rx_up_pro(struct hns_nic_ring_data *ring_data,
			      struct sk_buff *skb)
{
	struct net_device *ndev = ring_data->napi.dev;

	skb->protocol = eth_type_trans(skb, ndev);
	(void)napi_gro_receive(&ring_data->napi, skb);
}
