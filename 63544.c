static int cdrom_ioctl_multisession(struct cdrom_device_info *cdi,
		void __user *argp)
{
	struct cdrom_multisession ms_info;
	u8 requested_format;
	int ret;

	cd_dbg(CD_DO_IOCTL, "entering CDROMMULTISESSION\n");

	if (!(cdi->ops->capability & CDC_MULTI_SESSION))
		return -ENOSYS;

	if (copy_from_user(&ms_info, argp, sizeof(ms_info)))
		return -EFAULT;

	requested_format = ms_info.addr_format;
	if (requested_format != CDROM_MSF && requested_format != CDROM_LBA)
		return -EINVAL;
	ms_info.addr_format = CDROM_LBA;

	ret = cdi->ops->get_last_session(cdi, &ms_info);
	if (ret)
		return ret;

	sanitize_format(&ms_info.addr, &ms_info.addr_format, requested_format);

	if (copy_to_user(argp, &ms_info, sizeof(ms_info)))
		return -EFAULT;

	cd_dbg(CD_DO_IOCTL, "CDROMMULTISESSION successful\n");
	return 0;
}
