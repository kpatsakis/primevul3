static void hns_nic_net_timeout(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);

	hns_tx_timeout_reset(priv);
}
