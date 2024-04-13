int cdrom_mode_sense(struct cdrom_device_info *cdi,
		     struct packet_command *cgc,
		     int page_code, int page_control)
{
	const struct cdrom_device_ops *cdo = cdi->ops;

	memset(cgc->cmd, 0, sizeof(cgc->cmd));

	cgc->cmd[0] = GPCMD_MODE_SENSE_10;
	cgc->cmd[2] = page_code | (page_control << 6);
	cgc->cmd[7] = cgc->buflen >> 8;
	cgc->cmd[8] = cgc->buflen & 0xff;
	cgc->data_direction = CGC_DATA_READ;
	return cdo->generic_packet(cdi, cgc);
}
