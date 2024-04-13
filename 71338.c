megasas_page_size_show(struct device *cdev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%ld\n", (unsigned long)PAGE_SIZE - 1);
}
