support_nvme_encapsulation_show(struct device_driver *dd, char *buf)
{
	return sprintf(buf, "%u\n", support_nvme_encapsulation);
}
