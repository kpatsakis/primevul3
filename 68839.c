static void hns_disable_serdes_lb(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h = priv->ae_handle;
	struct hnae_ae_ops *ops = h->dev->ops;

	ops->stop(h);
	ops->set_loopback(h, MAC_INTERNALLOOP_SERDES, 0);
}
