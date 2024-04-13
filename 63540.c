static int cdrom_ioctl_get_mcn(struct cdrom_device_info *cdi,
		void __user *argp)
{
	struct cdrom_mcn mcn;
	int ret;

	cd_dbg(CD_DO_IOCTL, "entering CDROM_GET_MCN\n");

	if (!(cdi->ops->capability & CDC_MCN))
		return -ENOSYS;
	ret = cdi->ops->get_mcn(cdi, &mcn);
	if (ret)
		return ret;

	if (copy_to_user(argp, &mcn, sizeof(mcn)))
		return -EFAULT;
	cd_dbg(CD_DO_IOCTL, "CDROM_GET_MCN successful\n");
	return 0;
}
