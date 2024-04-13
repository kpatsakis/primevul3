static irqreturn_t hns_irq_handle(int irq, void *dev)
{
	struct hns_nic_ring_data *ring_data = (struct hns_nic_ring_data *)dev;

	ring_data->ring->q->handle->dev->ops->toggle_ring_irq(
		ring_data->ring, 1);
	napi_schedule(&ring_data->napi);

	return IRQ_HANDLED;
}
