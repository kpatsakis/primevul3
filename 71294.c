megasas_fw_crash_buffer_size_show(struct device *cdev,
	struct device_attribute *attr, char *buf)
{
	struct Scsi_Host *shost = class_to_shost(cdev);
	struct megasas_instance *instance =
		(struct megasas_instance *) shost->hostdata;

	return snprintf(buf, PAGE_SIZE, "%ld\n", (unsigned long)
		((instance->fw_crash_buffer_size) * 1024 * 1024)/PAGE_SIZE);
}
