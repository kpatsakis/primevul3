static void hns_nic_adjust_link(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h = priv->ae_handle;
	int state = 1;

	if (ndev->phydev) {
		h->dev->ops->adjust_link(h, ndev->phydev->speed,
					 ndev->phydev->duplex);
		state = ndev->phydev->link;
	}
	state = state && h->dev->ops->get_status(h);

	if (state != priv->link) {
		if (state) {
			netif_carrier_on(ndev);
			netif_tx_wake_all_queues(ndev);
			netdev_info(ndev, "link up\n");
		} else {
			netif_carrier_off(ndev);
			netdev_info(ndev, "link down\n");
		}
		priv->link = state;
	}
}
