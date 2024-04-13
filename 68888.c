static int hns_nic_uc_sync(struct net_device *netdev, const unsigned char *addr)
{
	struct hns_nic_priv *priv = netdev_priv(netdev);
	struct hnae_handle *h = priv->ae_handle;

	if (h->dev->ops->add_uc_addr)
		return h->dev->ops->add_uc_addr(h, addr);

	return 0;
}
