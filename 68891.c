static void hns_nic_update_link_status(struct net_device *netdev)
{
	struct hns_nic_priv *priv = netdev_priv(netdev);

	struct hnae_handle *h = priv->ae_handle;

	if (h->phy_dev) {
		if (h->phy_if != PHY_INTERFACE_MODE_XGMII)
			return;

		(void)genphy_read_status(h->phy_dev);
	}
	hns_nic_adjust_link(netdev);
}
