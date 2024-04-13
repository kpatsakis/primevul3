static int hns_enable_serdes_lb(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h = priv->ae_handle;
	struct hnae_ae_ops *ops = h->dev->ops;
	int speed, duplex;
	int ret;

	ret = ops->set_loopback(h, MAC_INTERNALLOOP_SERDES, 1);
	if (ret)
		return ret;

	ret = ops->start ? ops->start(h) : 0;
	if (ret)
		return ret;

	/* link adjust duplex*/
	if (h->phy_if != PHY_INTERFACE_MODE_XGMII)
		speed = 1000;
	else
		speed = 10000;
	duplex = 1;

	ops->adjust_link(h, speed, duplex);

	/* wait h/w ready */
	mdelay(300);

	return 0;
}
