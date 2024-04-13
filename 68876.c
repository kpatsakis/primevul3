static void hns_nic_service_event_complete(struct hns_nic_priv *priv)
{
	WARN_ON(!test_bit(NIC_STATE_SERVICE_SCHED, &priv->state));
	/* make sure to commit the things */
	smp_mb__before_atomic();
	clear_bit(NIC_STATE_SERVICE_SCHED, &priv->state);
}
