static int cdrom_ioctl_eject_sw(struct cdrom_device_info *cdi,
		unsigned long arg)
{
	cd_dbg(CD_DO_IOCTL, "entering CDROMEJECT_SW\n");

	if (!CDROM_CAN(CDC_OPEN_TRAY))
		return -ENOSYS;
	if (cdi->keeplocked)
		return -EBUSY;

	cdi->options &= ~(CDO_AUTO_CLOSE | CDO_AUTO_EJECT);
	if (arg)
		cdi->options |= CDO_AUTO_CLOSE | CDO_AUTO_EJECT;
	return 0;
}
