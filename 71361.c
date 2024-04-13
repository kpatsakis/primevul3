megasas_setup_jbod_map(struct megasas_instance *instance)
{
	int i;
	struct fusion_context *fusion = instance->ctrl_context;
	u32 pd_seq_map_sz;

	pd_seq_map_sz = sizeof(struct MR_PD_CFG_SEQ_NUM_SYNC) +
		(sizeof(struct MR_PD_CFG_SEQ) * (MAX_PHYSICAL_DEVICES - 1));

	if (reset_devices || !fusion ||
		!instance->ctrl_info_buf->adapterOperations3.useSeqNumJbodFP) {
		dev_info(&instance->pdev->dev,
			"Jbod map is not supported %s %d\n",
			__func__, __LINE__);
		instance->use_seqnum_jbod_fp = false;
		return;
	}

	if (fusion->pd_seq_sync[0])
		goto skip_alloc;

	for (i = 0; i < JBOD_MAPS_COUNT; i++) {
		fusion->pd_seq_sync[i] = dma_alloc_coherent
			(&instance->pdev->dev, pd_seq_map_sz,
			&fusion->pd_seq_phys[i], GFP_KERNEL);
		if (!fusion->pd_seq_sync[i]) {
			dev_err(&instance->pdev->dev,
				"Failed to allocate memory from %s %d\n",
				__func__, __LINE__);
			if (i == 1) {
				dma_free_coherent(&instance->pdev->dev,
					pd_seq_map_sz, fusion->pd_seq_sync[0],
					fusion->pd_seq_phys[0]);
				fusion->pd_seq_sync[0] = NULL;
			}
			instance->use_seqnum_jbod_fp = false;
			return;
		}
	}

skip_alloc:
	if (!megasas_sync_pd_seq_num(instance, false) &&
		!megasas_sync_pd_seq_num(instance, true))
		instance->use_seqnum_jbod_fp = true;
	else
		instance->use_seqnum_jbod_fp = false;
}
