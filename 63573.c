static int cdrom_read_subchannel(struct cdrom_device_info *cdi,
				 struct cdrom_subchnl *subchnl, int mcn)
{
	const struct cdrom_device_ops *cdo = cdi->ops;
	struct packet_command cgc;
	char buffer[32];
	int ret;

	init_cdrom_command(&cgc, buffer, 16, CGC_DATA_READ);
	cgc.cmd[0] = GPCMD_READ_SUBCHANNEL;
	cgc.cmd[1] = subchnl->cdsc_format;/* MSF or LBA addressing */
	cgc.cmd[2] = 0x40;  /* request subQ data */
	cgc.cmd[3] = mcn ? 2 : 1;
	cgc.cmd[8] = 16;

	if ((ret = cdo->generic_packet(cdi, &cgc)))
		return ret;

	subchnl->cdsc_audiostatus = cgc.buffer[1];
	subchnl->cdsc_ctrl = cgc.buffer[5] & 0xf;
	subchnl->cdsc_trk = cgc.buffer[6];
	subchnl->cdsc_ind = cgc.buffer[7];

	if (subchnl->cdsc_format == CDROM_LBA) {
		subchnl->cdsc_absaddr.lba = ((cgc.buffer[8] << 24) |
						(cgc.buffer[9] << 16) |
						(cgc.buffer[10] << 8) |
						(cgc.buffer[11]));
		subchnl->cdsc_reladdr.lba = ((cgc.buffer[12] << 24) |
						(cgc.buffer[13] << 16) |
						(cgc.buffer[14] << 8) |
						(cgc.buffer[15]));
	} else {
		subchnl->cdsc_reladdr.msf.minute = cgc.buffer[13];
		subchnl->cdsc_reladdr.msf.second = cgc.buffer[14];
		subchnl->cdsc_reladdr.msf.frame = cgc.buffer[15];
		subchnl->cdsc_absaddr.msf.minute = cgc.buffer[9];
		subchnl->cdsc_absaddr.msf.second = cgc.buffer[10];
		subchnl->cdsc_absaddr.msf.frame = cgc.buffer[11];
	}

	return 0;
}
