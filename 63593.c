static noinline int mmc_ioctl_cdrom_pause_resume(struct cdrom_device_info *cdi,
						 struct packet_command *cgc,
						 int cmd)
{
	const struct cdrom_device_ops *cdo = cdi->ops;
	cd_dbg(CD_DO_IOCTL, "entering CDROMPAUSE/CDROMRESUME\n");
	cgc->cmd[0] = GPCMD_PAUSE_RESUME;
	cgc->cmd[8] = (cmd == CDROMRESUME) ? 1 : 0;
	cgc->data_direction = CGC_DATA_NONE;
	return cdo->generic_packet(cdi, cgc);
}
