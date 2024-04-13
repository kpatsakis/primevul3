static int cdrom_ioctl_set_options(struct cdrom_device_info *cdi,
		unsigned long arg)
{
	cd_dbg(CD_DO_IOCTL, "entering CDROM_SET_OPTIONS\n");

	/*
	 * Options need to be in sync with capability.
	 * Too late for that, so we have to check each one separately.
	 */
	switch (arg) {
	case CDO_USE_FFLAGS:
	case CDO_CHECK_TYPE:
		break;
	case CDO_LOCK:
		if (!CDROM_CAN(CDC_LOCK))
			return -ENOSYS;
		break;
	case 0:
		return cdi->options;
	/* default is basically CDO_[AUTO_CLOSE|AUTO_EJECT] */
	default:
		if (!CDROM_CAN(arg))
			return -ENOSYS;
	}
	cdi->options |= (int) arg;
	return cdi->options;
}
