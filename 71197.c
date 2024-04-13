static ssize_t ipmi_type_show(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	struct smi_info *smi_info = dev_get_drvdata(dev);

	return snprintf(buf, 10, "%s\n", si_to_str[smi_info->io.si_type]);
}
