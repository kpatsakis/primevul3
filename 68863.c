static int hns_nic_net_up(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h = priv->ae_handle;
	int i, j;
	int ret;

	ret = hns_nic_init_irq(priv);
	if (ret != 0) {
		netdev_err(ndev, "hns init irq failed! ret=%d\n", ret);
		return ret;
	}

	for (i = 0; i < h->q_num * 2; i++) {
		ret = hns_nic_ring_open(ndev, i);
		if (ret)
			goto out_has_some_queues;
	}

	ret = h->dev->ops->set_mac_addr(h, ndev->dev_addr);
	if (ret)
		goto out_set_mac_addr_err;

	ret = h->dev->ops->start ? h->dev->ops->start(h) : 0;
	if (ret)
		goto out_start_err;

	if (ndev->phydev)
		phy_start(ndev->phydev);

	clear_bit(NIC_STATE_DOWN, &priv->state);
	(void)mod_timer(&priv->service_timer, jiffies + SERVICE_TIMER_HZ);

	return 0;

out_start_err:
	netif_stop_queue(ndev);
out_set_mac_addr_err:
out_has_some_queues:
	for (j = i - 1; j >= 0; j--)
		hns_nic_ring_close(ndev, j);

	set_bit(NIC_STATE_DOWN, &priv->state);

	return ret;
}
