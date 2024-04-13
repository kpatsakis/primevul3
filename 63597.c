static noinline int mmc_ioctl_cdrom_start_stop(struct cdrom_device_info *cdi,
					       struct packet_command *cgc,
					       int cmd)
{
	const struct cdrom_device_ops *cdo = cdi->ops;
	cd_dbg(CD_DO_IOCTL, "entering CDROMSTART/CDROMSTOP\n");
	cgc->cmd[0] = GPCMD_START_STOP_UNIT;
	cgc->cmd[1] = 1;
	cgc->cmd[4] = (cmd == CDROMSTART) ? 1 : 0;
	cgc->data_direction = CGC_DATA_NONE;
	return cdo->generic_packet(cdi, cgc);
}
