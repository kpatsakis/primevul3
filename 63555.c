static void cdrom_mmc3_profile(struct cdrom_device_info *cdi)
{
	struct packet_command cgc;
	char buffer[32];
	int ret, mmc3_profile;

	init_cdrom_command(&cgc, buffer, sizeof(buffer), CGC_DATA_READ);

	cgc.cmd[0] = GPCMD_GET_CONFIGURATION;
	cgc.cmd[1] = 0;
	cgc.cmd[2] = cgc.cmd[3] = 0;		/* Starting Feature Number */
	cgc.cmd[8] = sizeof(buffer);		/* Allocation Length */
	cgc.quiet = 1;

	if ((ret = cdi->ops->generic_packet(cdi, &cgc)))
		mmc3_profile = 0xffff;
	else
		mmc3_profile = (buffer[6] << 8) | buffer[7];

	cdi->mmc3_profile = mmc3_profile;
}
