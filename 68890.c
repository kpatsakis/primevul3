static void hns_nic_uninit_ring_data(struct hns_nic_priv *priv)
{
	struct hnae_handle *h = priv->ae_handle;
	int i;

	for (i = 0; i < h->q_num * 2; i++) {
		netif_napi_del(&priv->ring_data[i].napi);
		if (priv->ring_data[i].ring->irq_init_flag == RCB_IRQ_INITED) {
			(void)irq_set_affinity_hint(
				priv->ring_data[i].ring->irq,
				NULL);
			free_irq(priv->ring_data[i].ring->irq,
				 &priv->ring_data[i]);
		}

		priv->ring_data[i].ring->irq_init_flag = RCB_IRQ_NOT_INITED;
	}
	kfree(priv->ring_data);
}
