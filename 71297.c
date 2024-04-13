megasas_fw_crash_state_store(struct device *cdev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct Scsi_Host *shost = class_to_shost(cdev);
	struct megasas_instance *instance =
		(struct megasas_instance *) shost->hostdata;
	int val = 0;
	unsigned long flags;

	if (kstrtoint(buf, 0, &val) != 0)
		return -EINVAL;

	if ((val <= AVAILABLE || val > COPY_ERROR)) {
		dev_err(&instance->pdev->dev, "application updates invalid "
			"firmware crash state\n");
		return -EINVAL;
	}

	instance->fw_crash_state = val;

	if ((val == COPIED) || (val == COPY_ERROR)) {
		spin_lock_irqsave(&instance->crashdump_lock, flags);
		megasas_free_host_crash_buffer(instance);
		spin_unlock_irqrestore(&instance->crashdump_lock, flags);
		if (val == COPY_ERROR)
			dev_info(&instance->pdev->dev, "application failed to "
				"copy Firmware crash dump\n");
		else
			dev_info(&instance->pdev->dev, "Firmware crash dump "
				"copied successfully\n");
	}
	return strlen(buf);
}
