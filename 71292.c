megasas_fw_cmds_outstanding_show(struct device *cdev,
				 struct device_attribute *attr, char *buf)
{
	struct Scsi_Host *shost = class_to_shost(cdev);
	struct megasas_instance *instance = (struct megasas_instance *)shost->hostdata;

	return snprintf(buf, PAGE_SIZE, "%d\n", atomic_read(&instance->fw_outstanding));
}
