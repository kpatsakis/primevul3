static int cdrom_ram_open_write(struct cdrom_device_info *cdi)
{
	struct rwrt_feature_desc rfd;
	int ret;

	if ((ret = cdrom_has_defect_mgt(cdi)))
		return ret;

	if ((ret = cdrom_get_random_writable(cdi, &rfd)))
		return ret;
	else if (CDF_RWRT == be16_to_cpu(rfd.feature_code))
		ret = !rfd.curr;

	cd_dbg(CD_OPEN, "can open for random write\n");
	return ret;
}
