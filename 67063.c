static int sr_read_tochdr(struct cdrom_device_info *cdi,
		struct cdrom_tochdr *tochdr)
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
	cgc.cmd[8] = 12;		/* LSB of length */
	cgc.buffer = buffer;
	cgc.buflen = 12;
	cgc.quiet = 1;
	cgc.data_direction = DMA_FROM_DEVICE;

	result = sr_do_ioctl(cd, &cgc);

	tochdr->cdth_trk0 = buffer[2];
	tochdr->cdth_trk1 = buffer[3];

	kfree(buffer);
	return result;
}
