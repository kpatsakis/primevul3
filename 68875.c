hns_nic_select_queue(struct net_device *ndev, struct sk_buff *skb,
		     void *accel_priv, select_queue_fallback_t fallback)
{
	struct ethhdr *eth_hdr = (struct ethhdr *)skb->data;
	struct hns_nic_priv *priv = netdev_priv(ndev);

	/* fix hardware broadcast/multicast packets queue loopback */
	if (!AE_IS_VER1(priv->enet_ver) &&
	    is_multicast_ether_addr(eth_hdr->h_dest))
		return 0;
	else
		return fallback(ndev, skb);
}
