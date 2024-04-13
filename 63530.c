static int cdrom_ioctl_audioctl(struct cdrom_device_info *cdi,
		unsigned int cmd)
{
	int ret;

	cd_dbg(CD_DO_IOCTL, "doing audio ioctl (start/stop/pause/resume)\n");

	if (!CDROM_CAN(CDC_PLAY_AUDIO))
		return -ENOSYS;
	ret = check_for_audio_disc(cdi, cdi->ops);
	if (ret)
		return ret;
	return cdi->ops->audio_ioctl(cdi, cmd, NULL);
}
