static void hns_tx_timeout_reset(struct hns_nic_priv *priv)
{
	/* Do the reset outside of interrupt context */
	if (!test_bit(NIC_STATE_DOWN, &priv->state)) {
		set_bit(NIC_STATE2_RESET_REQUESTED, &priv->state);
		netdev_warn(priv->netdev,
			    "initiating reset due to tx timeout(%llu,0x%lx)\n",
			    priv->tx_timeout_count, priv->state);
		priv->tx_timeout_count++;
		hns_nic_task_schedule(priv);
	}
}
