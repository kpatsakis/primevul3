static void ccp_update_sg_workarea(struct ccp_sg_workarea *wa, unsigned int len)
{
	unsigned int nbytes = min_t(u64, len, wa->bytes_left);
	unsigned int sg_combined_len = 0;

	if (!wa->sg)
		return;

	wa->sg_used += nbytes;
	wa->bytes_left -= nbytes;
	if (wa->sg_used == sg_dma_len(wa->dma_sg)) {
		/* Advance to the next DMA scatterlist entry */
		wa->dma_sg = sg_next(wa->dma_sg);

		/* In the case that the DMA mapped scatterlist has entries
		 * that have been merged, the non-DMA mapped scatterlist
		 * must be advanced multiple times for each merged entry.
		 * This ensures that the current non-DMA mapped entry
		 * corresponds to the current DMA mapped entry.
		 */
		do {
			sg_combined_len += wa->sg->length;
			wa->sg = sg_next(wa->sg);
		} while (wa->sg_used > sg_combined_len);

		wa->sg_used = 0;
	}
}