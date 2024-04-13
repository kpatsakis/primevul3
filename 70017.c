static netdev_tx_t sit_tunnel_xmit(struct sk_buff *skb,
				   struct net_device *dev)
{
	if (!pskb_inet_may_pull(skb))
		goto tx_err;

	switch (skb->protocol) {
	case htons(ETH_P_IP):
		sit_tunnel_xmit__(skb, dev, IPPROTO_IPIP);
		break;
	case htons(ETH_P_IPV6):
		ipip6_tunnel_xmit(skb, dev);
		break;
#if IS_ENABLED(CONFIG_MPLS)
	case htons(ETH_P_MPLS_UC):
		sit_tunnel_xmit__(skb, dev, IPPROTO_MPLS);
		break;
#endif
	default:
		goto tx_err;
	}

	return NETDEV_TX_OK;

tx_err:
	dev->stats.tx_errors++;
	kfree_skb(skb);
	return NETDEV_TX_OK;

}
