static int cdrom_read_mech_status(struct cdrom_device_info *cdi, 
				  struct cdrom_changer_info *buf)
{
	struct packet_command cgc;
	const struct cdrom_device_ops *cdo = cdi->ops;
	int length;

	/*
	 * Sanyo changer isn't spec compliant (doesn't use regular change
	 * LOAD_UNLOAD command, and it doesn't implement the mech status
	 * command below
	 */
	if (cdi->sanyo_slot) {
		buf->hdr.nslots = 3;
		buf->hdr.curslot = cdi->sanyo_slot == 3 ? 0 : cdi->sanyo_slot;
		for (length = 0; length < 3; length++) {
			buf->slots[length].disc_present = 1;
			buf->slots[length].change = 0;
		}
		return 0;
	}

	length = sizeof(struct cdrom_mechstat_header) +
		 cdi->capacity * sizeof(struct cdrom_slot);

	init_cdrom_command(&cgc, buf, length, CGC_DATA_READ);
	cgc.cmd[0] = GPCMD_MECHANISM_STATUS;
	cgc.cmd[8] = (length >> 8) & 0xff;
	cgc.cmd[9] = length & 0xff;
	return cdo->generic_packet(cdi, &cgc);
}
