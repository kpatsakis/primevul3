static bool hns_nic_tx_fini_pro_v2(struct hns_nic_ring_data *ring_data)
{
	struct hnae_ring *ring = ring_data->ring;
	int head = readl_relaxed(ring->io_base + RCB_REG_HEAD);

	if (head == ring->next_to_clean)
		return true;
	else
		return false;
}
