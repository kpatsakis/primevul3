void hns_nic_poll_controller(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	unsigned long flags;
	int i;

	local_irq_save(flags);
	for (i = 0; i < priv->ae_handle->q_num * 2; i++)
		napi_schedule(&priv->ring_data[i].napi);
	local_irq_restore(flags);
}
