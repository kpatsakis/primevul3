static netdev_tx_t sit_tunnel_xmit__(struct sk_buff *skb,
				     struct net_device *dev, u8 ipproto)
{
	struct ip_tunnel *tunnel = netdev_priv(dev);
	const struct iphdr  *tiph = &tunnel->parms.iph;

	if (iptunnel_handle_offloads(skb, SKB_GSO_IPXIP4))
		goto tx_error;

	skb_set_inner_ipproto(skb, ipproto);

	ip_tunnel_xmit(skb, dev, tiph, ipproto);
	return NETDEV_TX_OK;
tx_error:
	kfree_skb(skb);
	dev->stats.tx_errors++;
	return NETDEV_TX_OK;
}
