static void hns_nic_ring_close(struct net_device *netdev, int idx)
{
	struct hns_nic_priv *priv = netdev_priv(netdev);
	struct hnae_handle *h = priv->ae_handle;

	h->dev->ops->toggle_ring_irq(priv->ring_data[idx].ring, 1);
	disable_irq(priv->ring_data[idx].ring->irq);

	napi_disable(&priv->ring_data[idx].napi);
}
