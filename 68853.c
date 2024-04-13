static int hns_nic_init_irq(struct hns_nic_priv *priv)
{
	struct hnae_handle *h = priv->ae_handle;
	struct hns_nic_ring_data *rd;
	int i;
	int ret;
	int cpu;

	for (i = 0; i < h->q_num * 2; i++) {
		rd = &priv->ring_data[i];

		if (rd->ring->irq_init_flag == RCB_IRQ_INITED)
			break;

		snprintf(rd->ring->ring_name, RCB_RING_NAME_LEN,
			 "%s-%s%d", priv->netdev->name,
			 (is_tx_ring(rd->ring) ? "tx" : "rx"), rd->queue_index);

		rd->ring->ring_name[RCB_RING_NAME_LEN - 1] = '\0';

		ret = request_irq(rd->ring->irq,
				  hns_irq_handle, 0, rd->ring->ring_name, rd);
		if (ret) {
			netdev_err(priv->netdev, "request irq(%d) fail\n",
				   rd->ring->irq);
			return ret;
		}
		disable_irq(rd->ring->irq);

		cpu = hns_nic_init_affinity_mask(h->q_num, i,
						 rd->ring, &rd->mask);

		if (cpu_online(cpu))
			irq_set_affinity_hint(rd->ring->irq,
					      &rd->mask);

		rd->ring->irq_init_flag = RCB_IRQ_INITED;
	}

	return 0;
}
