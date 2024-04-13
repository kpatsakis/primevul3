static int cdrom_mrw_bgformat_susp(struct cdrom_device_info *cdi, int immed)
{
	struct packet_command cgc;

	init_cdrom_command(&cgc, NULL, 0, CGC_DATA_NONE);
	cgc.cmd[0] = GPCMD_CLOSE_TRACK;

	/*
	 * Session = 1, Track = 0
	 */
	cgc.cmd[1] = !!immed;
	cgc.cmd[2] = 1 << 1;

	cgc.timeout = 5 * 60 * HZ;

	return cdi->ops->generic_packet(cdi, &cgc);
}
