static void hns_nic_dump(struct hns_nic_priv *priv)
{
	struct hnae_handle *h = priv->ae_handle;
	struct hnae_ae_ops *ops = h->dev->ops;
	u32 *data, reg_num, i;

	if (ops->get_regs_len && ops->get_regs) {
		reg_num = ops->get_regs_len(priv->ae_handle);
		reg_num = (reg_num + 3ul) & ~3ul;
		data = kcalloc(reg_num, sizeof(u32), GFP_KERNEL);
		if (data) {
			ops->get_regs(priv->ae_handle, data);
			for (i = 0; i < reg_num; i += 4)
				pr_info("0x%08x: 0x%08x 0x%08x 0x%08x 0x%08x\n",
					i, data[i], data[i + 1],
					data[i + 2], data[i + 3]);
			kfree(data);
		}
	}

	for (i = 0; i < h->q_num; i++) {
		pr_info("tx_queue%d_next_to_clean:%d\n",
			i, h->qs[i]->tx_ring.next_to_clean);
		pr_info("tx_queue%d_next_to_use:%d\n",
			i, h->qs[i]->tx_ring.next_to_use);
		pr_info("rx_queue%d_next_to_clean:%d\n",
			i, h->qs[i]->rx_ring.next_to_clean);
		pr_info("rx_queue%d_next_to_use:%d\n",
			i, h->qs[i]->rx_ring.next_to_use);
	}
}
