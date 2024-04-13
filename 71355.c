megasas_set_dma_mask(struct megasas_instance *instance)
{
	u64 consistent_mask;
	struct pci_dev *pdev;
	u32 scratch_pad_1;

	pdev = instance->pdev;
	consistent_mask = (instance->adapter_type >= VENTURA_SERIES) ?
				DMA_BIT_MASK(63) : DMA_BIT_MASK(32);

	if (IS_DMA64) {
		if (dma_set_mask(&pdev->dev, DMA_BIT_MASK(63)) &&
		    dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32)))
			goto fail_set_dma_mask;

		if ((*pdev->dev.dma_mask == DMA_BIT_MASK(63)) &&
		    (dma_set_coherent_mask(&pdev->dev, consistent_mask) &&
		     dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32)))) {
			/*
			 * If 32 bit DMA mask fails, then try for 64 bit mask
			 * for FW capable of handling 64 bit DMA.
			 */
			scratch_pad_1 = megasas_readl
				(instance, &instance->reg_set->outbound_scratch_pad_1);

			if (!(scratch_pad_1 & MR_CAN_HANDLE_64_BIT_DMA_OFFSET))
				goto fail_set_dma_mask;
			else if (dma_set_mask_and_coherent(&pdev->dev,
							   DMA_BIT_MASK(63)))
				goto fail_set_dma_mask;
		}
	} else if (dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32)))
		goto fail_set_dma_mask;

	if (pdev->dev.coherent_dma_mask == DMA_BIT_MASK(32))
		instance->consistent_mask_64bit = false;
	else
		instance->consistent_mask_64bit = true;

	dev_info(&pdev->dev, "%s bit DMA mask and %s bit consistent mask\n",
		 ((*pdev->dev.dma_mask == DMA_BIT_MASK(64)) ? "63" : "32"),
		 (instance->consistent_mask_64bit ? "63" : "32"));

	return 0;

fail_set_dma_mask:
	dev_err(&pdev->dev, "Failed to set DMA mask\n");
	return -1;

}
