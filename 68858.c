static int hns_nic_net_open(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h = priv->ae_handle;
	int ret;

	if (test_bit(NIC_STATE_TESTING, &priv->state))
		return -EBUSY;

	priv->link = 0;
	netif_carrier_off(ndev);

	ret = netif_set_real_num_tx_queues(ndev, h->q_num);
	if (ret < 0) {
		netdev_err(ndev, "netif_set_real_num_tx_queues fail, ret=%d!\n",
			   ret);
		return ret;
	}

	ret = netif_set_real_num_rx_queues(ndev, h->q_num);
	if (ret < 0) {
		netdev_err(ndev,
			   "netif_set_real_num_rx_queues fail, ret=%d!\n", ret);
		return ret;
	}

	ret = hns_nic_net_up(ndev);
	if (ret) {
		netdev_err(ndev,
			   "hns net up fail, ret=%d!\n", ret);
		return ret;
	}

	return 0;
}
