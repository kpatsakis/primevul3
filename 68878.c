static void hns_nic_service_timer(unsigned long data)
{
	struct hns_nic_priv *priv = (struct hns_nic_priv *)data;

	(void)mod_timer(&priv->service_timer, jiffies + SERVICE_TIMER_HZ);

	hns_nic_task_schedule(priv);
}
