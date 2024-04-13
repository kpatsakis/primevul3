void hns_nic_update_stats(struct net_device *netdev)
{
	struct hns_nic_priv *priv = netdev_priv(netdev);
	struct hnae_handle *h = priv->ae_handle;

	h->dev->ops->update_stats(h, &netdev->stats);
}
