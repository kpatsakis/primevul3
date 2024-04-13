megasas_set_nvme_device_properties(struct scsi_device *sdev, u32 max_io_size)
{
	struct megasas_instance *instance;
	u32 mr_nvme_pg_size;

	instance = (struct megasas_instance *)sdev->host->hostdata;
	mr_nvme_pg_size = max_t(u32, instance->nvme_page_size,
				MR_DEFAULT_NVME_PAGE_SIZE);

	blk_queue_max_hw_sectors(sdev->request_queue, (max_io_size / 512));

	blk_queue_flag_set(QUEUE_FLAG_NOMERGES, sdev->request_queue);
	blk_queue_virt_boundary(sdev->request_queue, mr_nvme_pg_size - 1);
}
