static void cdrom_dvd_rw_close_write(struct cdrom_device_info *cdi)
{
	struct packet_command cgc;

	if (cdi->mmc3_profile != 0x1a) {
		cd_dbg(CD_CLOSE, "%s: No DVD+RW\n", cdi->name);
		return;
	}

	if (!cdi->media_written) {
		cd_dbg(CD_CLOSE, "%s: DVD+RW media clean\n", cdi->name);
		return;
	}

	pr_info("%s: dirty DVD+RW media, \"finalizing\"\n", cdi->name);

	init_cdrom_command(&cgc, NULL, 0, CGC_DATA_NONE);
	cgc.cmd[0] = GPCMD_FLUSH_CACHE;
	cgc.timeout = 30*HZ;
	cdi->ops->generic_packet(cdi, &cgc);

	init_cdrom_command(&cgc, NULL, 0, CGC_DATA_NONE);
	cgc.cmd[0] = GPCMD_CLOSE_TRACK;
	cgc.timeout = 3000*HZ;
	cgc.quiet = 1;
	cdi->ops->generic_packet(cdi, &cgc);

	init_cdrom_command(&cgc, NULL, 0, CGC_DATA_NONE);
	cgc.cmd[0] = GPCMD_CLOSE_TRACK;
	cgc.cmd[2] = 2;	 /* Close session */
	cgc.quiet = 1;
	cgc.timeout = 3000*HZ;
	cdi->ops->generic_packet(cdi, &cgc);

	cdi->media_written = 0;
}
