void cdrom_release(struct cdrom_device_info *cdi, fmode_t mode)
{
	const struct cdrom_device_ops *cdo = cdi->ops;
	int opened_for_data;

	cd_dbg(CD_CLOSE, "entering cdrom_release\n");

	if (cdi->use_count > 0)
		cdi->use_count--;

	if (cdi->use_count == 0) {
		cd_dbg(CD_CLOSE, "Use count for \"/dev/%s\" now zero\n",
		       cdi->name);
		cdrom_dvd_rw_close_write(cdi);

		if ((cdo->capability & CDC_LOCK) && !cdi->keeplocked) {
			cd_dbg(CD_CLOSE, "Unlocking door!\n");
			cdo->lock_door(cdi, 0);
		}
	}

	opened_for_data = !(cdi->options & CDO_USE_FFLAGS) ||
		!(mode & FMODE_NDELAY);

	/*
	 * flush cache on last write release
	 */
	if (CDROM_CAN(CDC_RAM) && !cdi->use_count && cdi->for_data)
		cdrom_close_write(cdi);

	cdo->release(cdi);
	if (cdi->use_count == 0) {      /* last process that closes dev*/
		if (opened_for_data &&
		    cdi->options & CDO_AUTO_EJECT && CDROM_CAN(CDC_OPEN_TRAY))
			cdo->tray_move(cdi, 1);
	}
}
