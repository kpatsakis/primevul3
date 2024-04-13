static int hns_nic_set_features(struct net_device *netdev,
				netdev_features_t features)
{
	struct hns_nic_priv *priv = netdev_priv(netdev);

	switch (priv->enet_ver) {
	case AE_VERSION_1:
		if (features & (NETIF_F_TSO | NETIF_F_TSO6))
			netdev_info(netdev, "enet v1 do not support tso!\n");
		break;
	default:
		if (features & (NETIF_F_TSO | NETIF_F_TSO6)) {
			priv->ops.fill_desc = fill_tso_desc;
			priv->ops.maybe_stop_tx = hns_nic_maybe_stop_tso;
			/* The chip only support 7*4096 */
			netif_set_gso_max_size(netdev, 7 * 4096);
		} else {
			priv->ops.fill_desc = fill_v2_desc;
			priv->ops.maybe_stop_tx = hns_nic_maybe_stop_tx;
		}
		break;
	}
	netdev->features = features;
	return 0;
}
