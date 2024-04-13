static int hns_nic_dev_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct hns_nic_priv *priv = netdev_priv(ndev);

	if (ndev->reg_state != NETREG_UNINITIALIZED)
		unregister_netdev(ndev);

	if (priv->ring_data)
		hns_nic_uninit_ring_data(priv);
	priv->ring_data = NULL;

	if (ndev->phydev)
		phy_disconnect(ndev->phydev);

	if (!IS_ERR_OR_NULL(priv->ae_handle))
		hnae_put_handle(priv->ae_handle);
	priv->ae_handle = NULL;
	if (priv->notifier_block.notifier_call)
		hnae_unregister_notifier(&priv->notifier_block);
	priv->notifier_block.notifier_call = NULL;

	set_bit(NIC_STATE_REMOVING, &priv->state);
	(void)cancel_work_sync(&priv->service_task);

	free_netdev(ndev);
	return 0;
}
