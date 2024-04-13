megasas_mgmt_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case MEGASAS_IOC_FIRMWARE:
		return megasas_mgmt_ioctl_fw(file, arg);

	case MEGASAS_IOC_GET_AEN:
		return megasas_mgmt_ioctl_aen(file, arg);
	}

	return -ENOTTY;
}
