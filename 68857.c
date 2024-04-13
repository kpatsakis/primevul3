static void hns_nic_net_down(struct net_device *ndev)
{
	int i;
	struct hnae_ae_ops *ops;
	struct hns_nic_priv *priv = netdev_priv(ndev);

	if (test_and_set_bit(NIC_STATE_DOWN, &priv->state))
		return;

	(void)del_timer_sync(&priv->service_timer);
	netif_tx_stop_all_queues(ndev);
	netif_carrier_off(ndev);
	netif_tx_disable(ndev);
	priv->link = 0;

	if (ndev->phydev)
		phy_stop(ndev->phydev);

	ops = priv->ae_handle->dev->ops;

	if (ops->stop)
		ops->stop(priv->ae_handle);

	netif_tx_stop_all_queues(ndev);

	for (i = priv->ae_handle->q_num - 1; i >= 0; i--) {
		hns_nic_ring_close(ndev, i);
		hns_nic_ring_close(ndev, i + priv->ae_handle->q_num);

		/* clean tx buffers*/
		hns_nic_tx_clr_all_bufs(priv->ring_data + i);
	}
}
