static int cdrom_load_unload(struct cdrom_device_info *cdi, int slot) 
{
	struct packet_command cgc;

	cd_dbg(CD_CHANGER, "entering cdrom_load_unload()\n");
	if (cdi->sanyo_slot && slot < 0)
		return 0;

	init_cdrom_command(&cgc, NULL, 0, CGC_DATA_NONE);
	cgc.cmd[0] = GPCMD_LOAD_UNLOAD;
	cgc.cmd[4] = 2 + (slot >= 0);
	cgc.cmd[8] = slot;
	cgc.timeout = 60 * HZ;

	/* The Sanyo 3 CD changer uses byte 7 of the 
	GPCMD_TEST_UNIT_READY to command to switch CDs instead of
	using the GPCMD_LOAD_UNLOAD opcode. */
	if (cdi->sanyo_slot && -1 < slot) {
		cgc.cmd[0] = GPCMD_TEST_UNIT_READY;
		cgc.cmd[7] = slot;
		cgc.cmd[4] = cgc.cmd[8] = 0;
		cdi->sanyo_slot = slot ? slot : 3;
	}

	return cdi->ops->generic_packet(cdi, &cgc);
}
