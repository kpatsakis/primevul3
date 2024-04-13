static noinline int mmc_ioctl_cdrom_play_blk(struct cdrom_device_info *cdi,
					     void __user *arg,
					     struct packet_command *cgc)
{
	const struct cdrom_device_ops *cdo = cdi->ops;
	struct cdrom_blk blk;
	cd_dbg(CD_DO_IOCTL, "entering CDROMPLAYBLK\n");
	if (copy_from_user(&blk, (struct cdrom_blk __user *)arg, sizeof(blk)))
		return -EFAULT;
	cgc->cmd[0] = GPCMD_PLAY_AUDIO_10;
	cgc->cmd[2] = (blk.from >> 24) & 0xff;
	cgc->cmd[3] = (blk.from >> 16) & 0xff;
	cgc->cmd[4] = (blk.from >>  8) & 0xff;
	cgc->cmd[5] = blk.from & 0xff;
	cgc->cmd[7] = (blk.len >> 8) & 0xff;
	cgc->cmd[8] = blk.len & 0xff;
	cgc->data_direction = CGC_DATA_NONE;
	return cdo->generic_packet(cdi, cgc);
}
