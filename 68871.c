static bool hns_nic_rx_fini_pro(struct hns_nic_ring_data *ring_data)
{
	struct hnae_ring *ring = ring_data->ring;
	int num = 0;

	ring_data->ring->q->handle->dev->ops->toggle_ring_irq(ring, 0);

	/* for hardware bug fixed */
	num = readl_relaxed(ring->io_base + RCB_REG_FBDNUM);

	if (num > 0) {
		ring_data->ring->q->handle->dev->ops->toggle_ring_irq(
			ring_data->ring, 1);

		return false;
	} else {
		return true;
	}
}
