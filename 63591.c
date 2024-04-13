static int mmc_ioctl(struct cdrom_device_info *cdi, unsigned int cmd,
		     unsigned long arg)
{
	struct packet_command cgc;
	void __user *userptr = (void __user *)arg;

	memset(&cgc, 0, sizeof(cgc));

	/* build a unified command and queue it through
	   cdo->generic_packet() */
	switch (cmd) {
	case CDROMREADRAW:
	case CDROMREADMODE1:
	case CDROMREADMODE2:
		return mmc_ioctl_cdrom_read_data(cdi, userptr, &cgc, cmd);
	case CDROMREADAUDIO:
		return mmc_ioctl_cdrom_read_audio(cdi, userptr);
	case CDROMSUBCHNL:
		return mmc_ioctl_cdrom_subchannel(cdi, userptr);
	case CDROMPLAYMSF:
		return mmc_ioctl_cdrom_play_msf(cdi, userptr, &cgc);
	case CDROMPLAYBLK:
		return mmc_ioctl_cdrom_play_blk(cdi, userptr, &cgc);
	case CDROMVOLCTRL:
	case CDROMVOLREAD:
		return mmc_ioctl_cdrom_volume(cdi, userptr, &cgc, cmd);
	case CDROMSTART:
	case CDROMSTOP:
		return mmc_ioctl_cdrom_start_stop(cdi, &cgc, cmd);
	case CDROMPAUSE:
	case CDROMRESUME:
		return mmc_ioctl_cdrom_pause_resume(cdi, &cgc, cmd);
	case DVD_READ_STRUCT:
		return mmc_ioctl_dvd_read_struct(cdi, userptr, &cgc);
	case DVD_AUTH:
		return mmc_ioctl_dvd_auth(cdi, userptr);
	case CDROM_NEXT_WRITABLE:
		return mmc_ioctl_cdrom_next_writable(cdi, userptr);
	case CDROM_LAST_WRITTEN:
		return mmc_ioctl_cdrom_last_written(cdi, userptr);
	}

	return -ENOTTY;
}
