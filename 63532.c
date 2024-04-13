static int cdrom_ioctl_clear_options(struct cdrom_device_info *cdi,
		unsigned long arg)
{
	cd_dbg(CD_DO_IOCTL, "entering CDROM_CLEAR_OPTIONS\n");

	cdi->options &= ~(int) arg;
	return cdi->options;
}
