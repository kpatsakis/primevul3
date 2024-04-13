static int cdrom_get_track_info(struct cdrom_device_info *cdi,
				__u16 track, __u8 type, track_information *ti)
{
	const struct cdrom_device_ops *cdo = cdi->ops;
	struct packet_command cgc;
	int ret, buflen;

	init_cdrom_command(&cgc, ti, 8, CGC_DATA_READ);
	cgc.cmd[0] = GPCMD_READ_TRACK_RZONE_INFO;
	cgc.cmd[1] = type & 3;
	cgc.cmd[4] = (track & 0xff00) >> 8;
	cgc.cmd[5] = track & 0xff;
	cgc.cmd[8] = 8;
	cgc.quiet = 1;

	ret = cdo->generic_packet(cdi, &cgc);
	if (ret)
		return ret;

	buflen = be16_to_cpu(ti->track_information_length) +
		sizeof(ti->track_information_length);

	if (buflen > sizeof(track_information))
		buflen = sizeof(track_information);

	cgc.cmd[8] = cgc.buflen = buflen;
	ret = cdo->generic_packet(cdi, &cgc);
	if (ret)
		return ret;

	/* return actual fill size */
	return buflen;
}
