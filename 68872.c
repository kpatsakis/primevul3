static bool hns_nic_rx_fini_pro_v2(struct hns_nic_ring_data *ring_data)
{
	struct hnae_ring *ring = ring_data->ring;
	int num;

	num = readl_relaxed(ring->io_base + RCB_REG_FBDNUM);

	if (!num)
		return true;
	else
		return false;
}
