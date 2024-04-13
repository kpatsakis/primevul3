static int is_valid_clean_head(struct hnae_ring *ring, int h)
{
	int u = ring->next_to_use;
	int c = ring->next_to_clean;

	if (unlikely(h > ring->desc_num))
		return 0;

	assert(u > 0 && u < ring->desc_num);
	assert(c > 0 && c < ring->desc_num);
	assert(u != c && h != c); /* must be checked before call this func */

	return u > c ? (h > c && h <= u) : (h > c || h <= u);
}
