static int sr_read_tocentry(struct cdrom_device_info *cdi,
		struct cdrom_tocentry *tocentry)
{
	struct scsi_cd *cd = cdi->handle;
	struct packet_command cgc;
	int result;
	unsigned char *buffer;

	buffer = kmalloc(32, GFP_KERNEL | SR_GFP_DMA(cd));
	if (!buffer)
		return -ENOMEM;

	memset(&cgc, 0, sizeof(struct packet_command));
	cgc.timeout = IOCTL_TIMEOUT;
	cgc.cmd[0] = GPCMD_READ_TOC_PMA_ATIP;
	cgc.cmd[1] |= (tocentry->cdte_format == CDROM_MSF) ? 0x02 : 0;
	cgc.cmd[6] = tocentry->cdte_track;
	cgc.cmd[8] = 12;		/* LSB of length */
	cgc.buffer = buffer;
	cgc.buflen = 12;
	cgc.data_direction = DMA_FROM_DEVICE;

	result = sr_do_ioctl(cd, &cgc);

	tocentry->cdte_ctrl = buffer[5] & 0xf;
	tocentry->cdte_adr = buffer[5] >> 4;
	tocentry->cdte_datamode = (tocentry->cdte_ctrl & 0x04) ? 1 : 0;
	if (tocentry->cdte_format == CDROM_MSF) {
		tocentry->cdte_addr.msf.minute = buffer[9];
		tocentry->cdte_addr.msf.second = buffer[10];
		tocentry->cdte_addr.msf.frame = buffer[11];
	} else
		tocentry->cdte_addr.lba = (((((buffer[8] << 8) + buffer[9]) << 8)
			+ buffer[10]) << 8) + buffer[11];

	kfree(buffer);
	return result;
}
