static noinline int mmc_ioctl_cdrom_next_writable(struct cdrom_device_info *cdi,
						  void __user *arg)
{
	int ret;
	long next = 0;
	cd_dbg(CD_DO_IOCTL, "entering CDROM_NEXT_WRITABLE\n");
	ret = cdrom_get_next_writable(cdi, &next);
	if (ret)
		return ret;
	if (copy_to_user((long __user *)arg, &next, sizeof(next)))
		return -EFAULT;
	return 0;
}
