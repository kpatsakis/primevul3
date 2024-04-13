static int dvd_read_copyright(struct cdrom_device_info *cdi, dvd_struct *s,
				struct packet_command *cgc)
{
	int ret;
	u_char buf[8];
	const struct cdrom_device_ops *cdo = cdi->ops;

	init_cdrom_command(cgc, buf, sizeof(buf), CGC_DATA_READ);
	cgc->cmd[0] = GPCMD_READ_DVD_STRUCTURE;
	cgc->cmd[6] = s->copyright.layer_num;
	cgc->cmd[7] = s->type;
	cgc->cmd[8] = cgc->buflen >> 8;
	cgc->cmd[9] = cgc->buflen & 0xff;

	ret = cdo->generic_packet(cdi, cgc);
	if (ret)
		return ret;

	s->copyright.cpst = buf[4];
	s->copyright.rmi = buf[5];

	return 0;
}
