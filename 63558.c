static int cdrom_mrw_exit(struct cdrom_device_info *cdi)
{
	disc_information di;
	int ret;

	ret = cdrom_get_disc_info(cdi, &di);
	if (ret < 0 || ret < (int)offsetof(typeof(di),disc_type))
		return 1;

	ret = 0;
	if (di.mrw_status == CDM_MRW_BGFORMAT_ACTIVE) {
		pr_info("issuing MRW background format suspend\n");
		ret = cdrom_mrw_bgformat_susp(cdi, 0);
	}

	if (!ret && cdi->media_written)
		ret = cdrom_flush_cache(cdi);

	return ret;
}
