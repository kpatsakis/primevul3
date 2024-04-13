static netdev_features_t hns_nic_fix_features(
		struct net_device *netdev, netdev_features_t features)
{
	struct hns_nic_priv *priv = netdev_priv(netdev);

	switch (priv->enet_ver) {
	case AE_VERSION_1:
		features &= ~(NETIF_F_TSO | NETIF_F_TSO6 |
				NETIF_F_HW_VLAN_CTAG_FILTER);
		break;
	default:
		break;
	}
	return features;
}
