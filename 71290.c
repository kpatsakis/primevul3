void megasas_free_ctrl_dma_buffers(struct megasas_instance *instance)
{
	struct pci_dev *pdev = instance->pdev;
	struct fusion_context *fusion = instance->ctrl_context;

	if (instance->evt_detail)
		dma_free_coherent(&pdev->dev, sizeof(struct megasas_evt_detail),
				    instance->evt_detail,
				    instance->evt_detail_h);

	if (fusion && fusion->ioc_init_request)
		dma_free_coherent(&pdev->dev,
				  sizeof(struct MPI2_IOC_INIT_REQUEST),
				  fusion->ioc_init_request,
				  fusion->ioc_init_request_phys);

	if (instance->pd_list_buf)
		dma_free_coherent(&pdev->dev,
				    MEGASAS_MAX_PD * sizeof(struct MR_PD_LIST),
				    instance->pd_list_buf,
				    instance->pd_list_buf_h);

	if (instance->ld_list_buf)
		dma_free_coherent(&pdev->dev, sizeof(struct MR_LD_LIST),
				    instance->ld_list_buf,
				    instance->ld_list_buf_h);

	if (instance->ld_targetid_list_buf)
		dma_free_coherent(&pdev->dev, sizeof(struct MR_LD_TARGETID_LIST),
				    instance->ld_targetid_list_buf,
				    instance->ld_targetid_list_buf_h);

	if (instance->ctrl_info_buf)
		dma_free_coherent(&pdev->dev, sizeof(struct megasas_ctrl_info),
				    instance->ctrl_info_buf,
				    instance->ctrl_info_buf_h);

	if (instance->system_info_buf)
		dma_free_coherent(&pdev->dev, sizeof(struct MR_DRV_SYSTEM_INFO),
				    instance->system_info_buf,
				    instance->system_info_h);

	if (instance->pd_info)
		dma_free_coherent(&pdev->dev, sizeof(struct MR_PD_INFO),
				    instance->pd_info, instance->pd_info_h);

	if (instance->tgt_prop)
		dma_free_coherent(&pdev->dev, sizeof(struct MR_TARGET_PROPERTIES),
				    instance->tgt_prop, instance->tgt_prop_h);

	if (instance->crash_dump_buf)
		dma_free_coherent(&pdev->dev, CRASH_DMA_BUF_SIZE,
				    instance->crash_dump_buf,
				    instance->crash_dump_h);

	if (instance->snapdump_prop)
		dma_free_coherent(&pdev->dev,
				  sizeof(struct MR_SNAPDUMP_PROPERTIES),
				  instance->snapdump_prop,
				  instance->snapdump_prop_h);

	if (instance->host_device_list_buf)
		dma_free_coherent(&pdev->dev,
				  HOST_DEVICE_LIST_SZ,
				  instance->host_device_list_buf,
				  instance->host_device_list_buf_h);

}
