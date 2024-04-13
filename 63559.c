static int cdrom_mrw_open_write(struct cdrom_device_info *cdi)
{
	disc_information di;
	int ret;

	/*
	 * always reset to DMA lba space on open
	 */
	if (cdrom_mrw_set_lba_space(cdi, MRW_LBA_DMA)) {
		pr_err("failed setting lba address space\n");
		return 1;
	}

	ret = cdrom_get_disc_info(cdi, &di);
	if (ret < 0 || ret < offsetof(typeof(di),disc_type))
		return 1;

	if (!di.erasable)
		return 1;

	/*
	 * mrw_status
	 * 0	-	not MRW formatted
	 * 1	-	MRW bgformat started, but not running or complete
	 * 2	-	MRW bgformat in progress
	 * 3	-	MRW formatting complete
	 */
	ret = 0;
	pr_info("open: mrw_status '%s'\n", mrw_format_status[di.mrw_status]);
	if (!di.mrw_status)
		ret = 1;
	else if (di.mrw_status == CDM_MRW_BGFORMAT_INACTIVE &&
			mrw_format_restart)
		ret = cdrom_mrw_bgformat(cdi, 1);

	return ret;
}
