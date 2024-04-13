static void ccp_dm_free(struct ccp_dm_workarea *wa)
{
	if (wa->length <= CCP_DMAPOOL_MAX_SIZE) {
		if (wa->address)
			dma_pool_free(wa->dma_pool, wa->address,
				      wa->dma.address);
	} else {
		if (wa->dma.address)
			dma_unmap_single(wa->dev, wa->dma.address, wa->length,
					 wa->dma.dir);
		kfree(wa->address);
	}

	wa->address = NULL;
	wa->dma.address = 0;
}