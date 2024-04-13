void hns_set_multicast_list(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h = priv->ae_handle;
	struct netdev_hw_addr *ha = NULL;

	if (!h)	{
		netdev_err(ndev, "hnae handle is null\n");
		return;
	}

	if (h->dev->ops->clr_mc_addr)
		if (h->dev->ops->clr_mc_addr(h))
			netdev_err(ndev, "clear multicast address fail\n");

	if (h->dev->ops->set_mc_addr) {
		netdev_for_each_mc_addr(ha, ndev)
			if (h->dev->ops->set_mc_addr(h, ha->addr))
				netdev_err(ndev, "set multicast fail\n");
	}
}
