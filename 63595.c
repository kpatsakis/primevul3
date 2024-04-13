static noinline int mmc_ioctl_cdrom_play_msf(struct cdrom_device_info *cdi,
					     void __user *arg,
					     struct packet_command *cgc)
{
	const struct cdrom_device_ops *cdo = cdi->ops;
	struct cdrom_msf msf;
	cd_dbg(CD_DO_IOCTL, "entering CDROMPLAYMSF\n");
	if (copy_from_user(&msf, (struct cdrom_msf __user *)arg, sizeof(msf)))
		return -EFAULT;
	cgc->cmd[0] = GPCMD_PLAY_AUDIO_MSF;
	cgc->cmd[3] = msf.cdmsf_min0;
	cgc->cmd[4] = msf.cdmsf_sec0;
	cgc->cmd[5] = msf.cdmsf_frame0;
	cgc->cmd[6] = msf.cdmsf_min1;
	cgc->cmd[7] = msf.cdmsf_sec1;
	cgc->cmd[8] = msf.cdmsf_frame1;
	cgc->data_direction = CGC_DATA_NONE;
	return cdo->generic_packet(cdi, cgc);
}
