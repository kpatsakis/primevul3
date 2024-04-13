int register_cdrom(struct cdrom_device_info *cdi)
{
	static char banner_printed;
	const struct cdrom_device_ops *cdo = cdi->ops;
	int *change_capability = (int *)&cdo->capability; /* hack */

	cd_dbg(CD_OPEN, "entering register_cdrom\n");

	if (cdo->open == NULL || cdo->release == NULL)
		return -EINVAL;
	if (!banner_printed) {
		pr_info("Uniform CD-ROM driver " REVISION "\n");
		banner_printed = 1;
		cdrom_sysctl_register();
	}

	ENSURE(drive_status, CDC_DRIVE_STATUS);
	if (cdo->check_events == NULL && cdo->media_changed == NULL)
		*change_capability = ~(CDC_MEDIA_CHANGED | CDC_SELECT_DISC);
	ENSURE(tray_move, CDC_CLOSE_TRAY | CDC_OPEN_TRAY);
	ENSURE(lock_door, CDC_LOCK);
	ENSURE(select_speed, CDC_SELECT_SPEED);
	ENSURE(get_last_session, CDC_MULTI_SESSION);
	ENSURE(get_mcn, CDC_MCN);
	ENSURE(reset, CDC_RESET);
	ENSURE(generic_packet, CDC_GENERIC_PACKET);
	cdi->mc_flags = 0;
	cdi->options = CDO_USE_FFLAGS;

	if (autoclose == 1 && CDROM_CAN(CDC_CLOSE_TRAY))
		cdi->options |= (int) CDO_AUTO_CLOSE;
	if (autoeject == 1 && CDROM_CAN(CDC_OPEN_TRAY))
		cdi->options |= (int) CDO_AUTO_EJECT;
	if (lockdoor == 1)
		cdi->options |= (int) CDO_LOCK;
	if (check_media_type == 1)
		cdi->options |= (int) CDO_CHECK_TYPE;

	if (CDROM_CAN(CDC_MRW_W))
		cdi->exit = cdrom_mrw_exit;

	if (cdi->disk)
		cdi->cdda_method = CDDA_BPC_FULL;
	else
		cdi->cdda_method = CDDA_OLD;

	WARN_ON(!cdo->generic_packet);

	cd_dbg(CD_REG_UNREG, "drive \"/dev/%s\" registered\n", cdi->name);
	mutex_lock(&cdrom_mutex);
	list_add(&cdi->list, &cdrom_list);
	mutex_unlock(&cdrom_mutex);
	return 0;
}
