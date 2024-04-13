megasas_fw_crash_buffer_show(struct device *cdev,
	struct device_attribute *attr, char *buf)
{
	struct Scsi_Host *shost = class_to_shost(cdev);
	struct megasas_instance *instance =
		(struct megasas_instance *) shost->hostdata;
	u32 size;
	unsigned long buff_addr;
	unsigned long dmachunk = CRASH_DMA_BUF_SIZE;
	unsigned long src_addr;
	unsigned long flags;
	u32 buff_offset;

	spin_lock_irqsave(&instance->crashdump_lock, flags);
	buff_offset = instance->fw_crash_buffer_offset;
	if (!instance->crash_dump_buf &&
		!((instance->fw_crash_state == AVAILABLE) ||
		(instance->fw_crash_state == COPYING))) {
		dev_err(&instance->pdev->dev,
			"Firmware crash dump is not available\n");
		spin_unlock_irqrestore(&instance->crashdump_lock, flags);
		return -EINVAL;
	}

	buff_addr = (unsigned long) buf;

	if (buff_offset > (instance->fw_crash_buffer_size * dmachunk)) {
		dev_err(&instance->pdev->dev,
			"Firmware crash dump offset is out of range\n");
		spin_unlock_irqrestore(&instance->crashdump_lock, flags);
		return 0;
	}

	size = (instance->fw_crash_buffer_size * dmachunk) - buff_offset;
	size = (size >= PAGE_SIZE) ? (PAGE_SIZE - 1) : size;

	src_addr = (unsigned long)instance->crash_buf[buff_offset / dmachunk] +
		(buff_offset % dmachunk);
	memcpy(buf, (void *)src_addr, size);
	spin_unlock_irqrestore(&instance->crashdump_lock, flags);

	return size;
}
