static void fill_tso_desc(struct hnae_ring *ring, void *priv,
			  int size, dma_addr_t dma, int frag_end,
			  int buf_num, enum hns_desc_type type, int mtu)
{
	int frag_buf_num;
	int sizeoflast;
	int k;

	frag_buf_num = (size + BD_MAX_SEND_SIZE - 1) / BD_MAX_SEND_SIZE;
	sizeoflast = size % BD_MAX_SEND_SIZE;
	sizeoflast = sizeoflast ? sizeoflast : BD_MAX_SEND_SIZE;

	/* when the frag size is bigger than hardware, split this frag */
	for (k = 0; k < frag_buf_num; k++)
		fill_v2_desc(ring, priv,
			     (k == frag_buf_num - 1) ?
					sizeoflast : BD_MAX_SEND_SIZE,
			     dma + BD_MAX_SEND_SIZE * k,
			     frag_end && (k == frag_buf_num - 1) ? 1 : 0,
			     buf_num,
			     (type == DESC_TYPE_SKB && !k) ?
					DESC_TYPE_SKB : DESC_TYPE_PAGE,
 			     mtu);
 }
