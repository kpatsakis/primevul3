int sr_get_mcn(struct cdrom_device_info *cdi, struct cdrom_mcn *mcn)
{
	Scsi_CD *cd = cdi->handle;
	struct packet_command cgc;
	char *buffer = kmalloc(32, GFP_KERNEL | SR_GFP_DMA(cd));
	int result;

	if (!buffer)
		return -ENOMEM;

	memset(&cgc, 0, sizeof(struct packet_command));
	cgc.cmd[0] = GPCMD_READ_SUBCHANNEL;
	cgc.cmd[2] = 0x40;	/* I do want the subchannel info */
	cgc.cmd[3] = 0x02;	/* Give me medium catalog number info */
	cgc.cmd[8] = 24;
	cgc.buffer = buffer;
	cgc.buflen = 24;
	cgc.data_direction = DMA_FROM_DEVICE;
	cgc.timeout = IOCTL_TIMEOUT;
	result = sr_do_ioctl(cd, &cgc);

	memcpy(mcn->medium_catalog_number, buffer + 9, 13);
	mcn->medium_catalog_number[13] = 0;

	kfree(buffer);
	return result;
}
