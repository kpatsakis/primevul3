static void hns_nic_service_task(struct work_struct *work)
{
	struct hns_nic_priv *priv
		= container_of(work, struct hns_nic_priv, service_task);
	struct hnae_handle *h = priv->ae_handle;

	hns_nic_update_link_status(priv->netdev);
	h->dev->ops->update_led_status(h);
	hns_nic_update_stats(priv->netdev);

	hns_nic_reset_subtask(priv);
	hns_nic_service_event_complete(priv);
}
