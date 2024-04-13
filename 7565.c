static void ccp_sg_free(struct ccp_sg_workarea *wa)
{
	if (wa->dma_count)
		dma_unmap_sg(wa->dma_dev, wa->dma_sg_head, wa->nents, wa->dma_dir);

	wa->dma_count = 0;
}