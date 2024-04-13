static void hns_nic_reset_subtask(struct hns_nic_priv *priv)
{
	enum hnae_port_type type = priv->ae_handle->port_type;

	if (!test_bit(NIC_STATE2_RESET_REQUESTED, &priv->state))
		return;
	clear_bit(NIC_STATE2_RESET_REQUESTED, &priv->state);

	/* If we're already down, removing or resetting, just bail */
	if (test_bit(NIC_STATE_DOWN, &priv->state) ||
	    test_bit(NIC_STATE_REMOVING, &priv->state) ||
	    test_bit(NIC_STATE_RESETTING, &priv->state))
		return;

	hns_nic_dump(priv);
	netdev_info(priv->netdev, "try to reset %s port!\n",
		    (type == HNAE_PORT_DEBUG ? "debug" : "service"));

	rtnl_lock();
	/* put off any impending NetWatchDogTimeout */
	netif_trans_update(priv->netdev);

	if (type == HNAE_PORT_DEBUG) {
		hns_nic_net_reinit(priv->netdev);
	} else {
		netif_carrier_off(priv->netdev);
		netif_tx_disable(priv->netdev);
	}
	rtnl_unlock();
}
