static int cdrom_get_random_writable(struct cdrom_device_info *cdi,
			      struct rwrt_feature_desc *rfd)
{
	struct packet_command cgc;
	char buffer[24];
	int ret;

	init_cdrom_command(&cgc, buffer, sizeof(buffer), CGC_DATA_READ);

	cgc.cmd[0] = GPCMD_GET_CONFIGURATION;	/* often 0x46 */
	cgc.cmd[3] = CDF_RWRT;			/* often 0x0020 */
	cgc.cmd[8] = sizeof(buffer);		/* often 0x18 */
	cgc.quiet = 1;

	if ((ret = cdi->ops->generic_packet(cdi, &cgc)))
		return ret;

	memcpy(rfd, &buffer[sizeof(struct feature_header)], sizeof (*rfd));
	return 0;
}
