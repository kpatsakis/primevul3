static int hns_nic_notifier_action(struct notifier_block *nb,
				   unsigned long action, void *data)
{
	struct hns_nic_priv *priv =
		container_of(nb, struct hns_nic_priv, notifier_block);

	assert(action == HNAE_AE_REGISTER);

	if (!hns_nic_try_get_ae(priv->netdev)) {
		hnae_unregister_notifier(&priv->notifier_block);
		priv->notifier_block.notifier_call = NULL;
	}
	return 0;
}
