static int hns_desc_unused(struct hnae_ring *ring)
{
	int ntc = ring->next_to_clean;
	int ntu = ring->next_to_use;

	return ((ntc >= ntu) ? 0 : ring->desc_num) + ntc - ntu;
}
