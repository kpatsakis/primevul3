static int cdrom_ioctl_closetray(struct cdrom_device_info *cdi)
{
	cd_dbg(CD_DO_IOCTL, "entering CDROMCLOSETRAY\n");

	if (!CDROM_CAN(CDC_CLOSE_TRAY))
		return -ENOSYS;
	return cdi->ops->tray_move(cdi, 0);
}
