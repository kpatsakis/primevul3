static void cdrom_update_settings(void)
{
	struct cdrom_device_info *cdi;

	mutex_lock(&cdrom_mutex);
	list_for_each_entry(cdi, &cdrom_list, list) {
		if (autoclose && CDROM_CAN(CDC_CLOSE_TRAY))
			cdi->options |= CDO_AUTO_CLOSE;
		else if (!autoclose)
			cdi->options &= ~CDO_AUTO_CLOSE;
		if (autoeject && CDROM_CAN(CDC_OPEN_TRAY))
			cdi->options |= CDO_AUTO_EJECT;
		else if (!autoeject)
			cdi->options &= ~CDO_AUTO_EJECT;
		if (lockdoor && CDROM_CAN(CDC_LOCK))
			cdi->options |= CDO_LOCK;
		else if (!lockdoor)
			cdi->options &= ~CDO_LOCK;
		if (check_media_type)
			cdi->options |= CDO_CHECK_TYPE;
		else
			cdi->options &= ~CDO_CHECK_TYPE;
	}
	mutex_unlock(&cdrom_mutex);
}
