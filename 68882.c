static void hns_nic_task_schedule(struct hns_nic_priv *priv)
{
	if (!test_bit(NIC_STATE_DOWN, &priv->state) &&
	    !test_bit(NIC_STATE_REMOVING, &priv->state) &&
	    !test_and_set_bit(NIC_STATE_SERVICE_SCHED, &priv->state))
		(void)schedule_work(&priv->service_task);
}
