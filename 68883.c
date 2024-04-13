static int hns_nic_try_get_ae(struct net_device *ndev)
{
	struct hns_nic_priv *priv = netdev_priv(ndev);
	struct hnae_handle *h;
	int ret;

	h = hnae_get_handle(&priv->netdev->dev,
			    priv->fwnode, priv->port_id, NULL);
	if (IS_ERR_OR_NULL(h)) {
		ret = -ENODEV;
		dev_dbg(priv->dev, "has not handle, register notifier!\n");
		goto out;
	}
	priv->ae_handle = h;

	ret = hns_nic_init_phy(ndev, h);
	if (ret) {
		dev_err(priv->dev, "probe phy device fail!\n");
		goto out_init_phy;
	}

	ret = hns_nic_init_ring_data(priv);
	if (ret) {
		ret = -ENOMEM;
		goto out_init_ring_data;
	}

	hns_nic_set_priv_ops(ndev);

	ret = register_netdev(ndev);
	if (ret) {
		dev_err(priv->dev, "probe register netdev fail!\n");
		goto out_reg_ndev_fail;
	}
	return 0;

out_reg_ndev_fail:
	hns_nic_uninit_ring_data(priv);
	priv->ring_data = NULL;
out_init_phy:
out_init_ring_data:
	hnae_put_handle(priv->ae_handle);
	priv->ae_handle = NULL;
out:
	return ret;
}
