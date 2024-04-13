static int cdrom_ioctl_volctrl(struct cdrom_device_info *cdi,
		void __user *argp)
{
	struct cdrom_volctrl volume;

	cd_dbg(CD_DO_IOCTL, "entering CDROMVOLCTRL\n");

	if (!CDROM_CAN(CDC_PLAY_AUDIO))
		return -ENOSYS;
	if (copy_from_user(&volume, argp, sizeof(volume)))
		return -EFAULT;
	return cdi->ops->audio_ioctl(cdi, CDROMVOLCTRL, &volume);
}
