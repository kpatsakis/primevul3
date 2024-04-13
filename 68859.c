void hns_nic_net_reinit(struct net_device *netdev)
{
	struct hns_nic_priv *priv = netdev_priv(netdev);

	netif_trans_update(priv->netdev);
	while (test_and_set_bit(NIC_STATE_REINITING, &priv->state))
		usleep_range(1000, 2000);

	hns_nic_net_down(netdev);
	hns_nic_net_reset(netdev);
	(void)hns_nic_net_up(netdev);
	clear_bit(NIC_STATE_REINITING, &priv->state);
}
