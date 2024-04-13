megasas_fw_crash_buffer_store(struct device *cdev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct Scsi_Host *shost = class_to_shost(cdev);
	struct megasas_instance *instance =
		(struct megasas_instance *) shost->hostdata;
	int val = 0;
	unsigned long flags;

	if (kstrtoint(buf, 0, &val) != 0)
		return -EINVAL;

	spin_lock_irqsave(&instance->crashdump_lock, flags);
	instance->fw_crash_buffer_offset = val;
	spin_unlock_irqrestore(&instance->crashdump_lock, flags);
	return strlen(buf);
}
