static ssize_t ipmi_interrupts_enabled_show(struct device *dev,
					    struct device_attribute *attr,
					    char *buf)
{
	struct smi_info *smi_info = dev_get_drvdata(dev);
	int enabled = smi_info->io.irq && !smi_info->interrupt_disabled;

	return snprintf(buf, 10, "%d\n", enabled);
}
