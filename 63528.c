static int cdrom_has_defect_mgt(struct cdrom_device_info *cdi)
{
	struct packet_command cgc;
	char buffer[16];
	__be16 *feature_code;
	int ret;

	init_cdrom_command(&cgc, buffer, sizeof(buffer), CGC_DATA_READ);

	cgc.cmd[0] = GPCMD_GET_CONFIGURATION;
	cgc.cmd[3] = CDF_HWDM;
	cgc.cmd[8] = sizeof(buffer);
	cgc.quiet = 1;

	if ((ret = cdi->ops->generic_packet(cdi, &cgc)))
		return ret;

	feature_code = (__be16 *) &buffer[sizeof(struct feature_header)];
	if (be16_to_cpu(*feature_code) == CDF_HWDM)
		return 0;

	return 1;
}
