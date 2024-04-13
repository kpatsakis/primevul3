static int ccp_init_dm_workarea(struct ccp_dm_workarea *wa,
				struct ccp_cmd_queue *cmd_q,
				unsigned int len,
				enum dma_data_direction dir)
{
	memset(wa, 0, sizeof(*wa));

	if (!len)
		return 0;

	wa->dev = cmd_q->ccp->dev;
	wa->length = len;

	if (len <= CCP_DMAPOOL_MAX_SIZE) {
		wa->dma_pool = cmd_q->dma_pool;

		wa->address = dma_pool_zalloc(wa->dma_pool, GFP_KERNEL,
					     &wa->dma.address);
		if (!wa->address)
			return -ENOMEM;

		wa->dma.length = CCP_DMAPOOL_MAX_SIZE;

	} else {
		wa->address = kzalloc(len, GFP_KERNEL);
		if (!wa->address)
			return -ENOMEM;

		wa->dma.address = dma_map_single(wa->dev, wa->address, len,
						 dir);
		if (dma_mapping_error(wa->dev, wa->dma.address))
			return -ENOMEM;

		wa->dma.length = len;
	}
	wa->dma.dir = dir;

	return 0;
}