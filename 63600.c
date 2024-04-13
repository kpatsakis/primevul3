static int mo_open_write(struct cdrom_device_info *cdi)
{
	struct packet_command cgc;
	char buffer[255];
	int ret;

	init_cdrom_command(&cgc, &buffer, 4, CGC_DATA_READ);
	cgc.quiet = 1;

	/*
	 * obtain write protect information as per
	 * drivers/scsi/sd.c:sd_read_write_protect_flag
	 */

	ret = cdrom_mode_sense(cdi, &cgc, GPMODE_ALL_PAGES, 0);
	if (ret)
		ret = cdrom_mode_sense(cdi, &cgc, GPMODE_VENDOR_PAGE, 0);
	if (ret) {
		cgc.buflen = 255;
		ret = cdrom_mode_sense(cdi, &cgc, GPMODE_ALL_PAGES, 0);
	}

	/* drive gave us no info, let the user go ahead */
	if (ret)
		return 0;

	return buffer[3] & 0x80;
}
