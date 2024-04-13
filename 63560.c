static int cdrom_mrw_probe_pc(struct cdrom_device_info *cdi)
{
	struct packet_command cgc;
	char buffer[16];

	init_cdrom_command(&cgc, buffer, sizeof(buffer), CGC_DATA_READ);

	cgc.timeout = HZ;
	cgc.quiet = 1;

	if (!cdrom_mode_sense(cdi, &cgc, MRW_MODE_PC, 0)) {
		cdi->mrw_mode_page = MRW_MODE_PC;
		return 0;
	} else if (!cdrom_mode_sense(cdi, &cgc, MRW_MODE_PC_PRE1, 0)) {
		cdi->mrw_mode_page = MRW_MODE_PC_PRE1;
		return 0;
	}

	return 1;
}
