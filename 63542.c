static int cdrom_ioctl_lock_door(struct cdrom_device_info *cdi,
		unsigned long arg)
{
	cd_dbg(CD_DO_IOCTL, "%socking door\n", arg ? "L" : "Unl");

	if (!CDROM_CAN(CDC_LOCK))
		return -EDRIVE_CANT_DO_THIS;

	cdi->keeplocked = arg ? 1 : 0;

	/*
	 * Don't unlock the door on multiple opens by default, but allow
	 * root to do so.
	 */
	if (cdi->use_count != 1 && !arg && !capable(CAP_SYS_ADMIN))
		return -EBUSY;
	return cdi->ops->lock_door(cdi, arg);
}
