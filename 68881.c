void hns_nic_set_rx_mode(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h = priv->ae_handle;

	if (h->dev->ops->set_promisc_mode) {
		if (ndev->flags & IFF_PROMISC)
			h->dev->ops->set_promisc_mode(h, 1);
		else
			h->dev->ops->set_promisc_mode(h, 0);
	}

	hns_set_multicast_list(ndev);

	if (__dev_uc_sync(ndev, hns_nic_uc_sync, hns_nic_uc_unsync))
		netdev_err(ndev, "sync uc address fail\n");
}
