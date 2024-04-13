static int cdrom_ioctl_play_msf(struct cdrom_device_info *cdi,
		void __user *argp)
{
	struct cdrom_msf msf;

	cd_dbg(CD_DO_IOCTL, "entering CDROMPLAYMSF\n");

	if (!CDROM_CAN(CDC_PLAY_AUDIO))
		return -ENOSYS;
	if (copy_from_user(&msf, argp, sizeof(msf)))
		return -EFAULT;
	return cdi->ops->audio_ioctl(cdi, CDROMPLAYMSF, &msf);
}
