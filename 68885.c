static bool hns_nic_tx_fini_pro(struct hns_nic_ring_data *ring_data)
{
	struct hnae_ring *ring = ring_data->ring;
	int head;

	ring_data->ring->q->handle->dev->ops->toggle_ring_irq(ring, 0);

	head = readl_relaxed(ring->io_base + RCB_REG_HEAD);

	if (head != ring->next_to_clean) {
		ring_data->ring->q->handle->dev->ops->toggle_ring_irq(
			ring_data->ring, 1);

		return false;
	} else {
		return true;
	}
}
