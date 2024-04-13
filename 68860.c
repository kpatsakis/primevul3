void hns_nic_net_reset(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *handle = priv->ae_handle;

	while (test_and_set_bit(NIC_STATE_RESETTING, &priv->state))
		usleep_range(1000, 2000);

	(void)hnae_reinit_handle(handle);

	clear_bit(NIC_STATE_RESETTING, &priv->state);
}
