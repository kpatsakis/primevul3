static int cdrom_ioctl_volread(struct cdrom_device_info *cdi,
		void __user *argp)
{
	struct cdrom_volctrl volume;
	int ret;

	cd_dbg(CD_DO_IOCTL, "entering CDROMVOLREAD\n");

	if (!CDROM_CAN(CDC_PLAY_AUDIO))
		return -ENOSYS;

	ret = cdi->ops->audio_ioctl(cdi, CDROMVOLREAD, &volume);
	if (ret)
		return ret;

	if (copy_to_user(argp, &volume, sizeof(volume)))
		return -EFAULT;
	return 0;
}
