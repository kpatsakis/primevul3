static int cdrom_ioctl_debug(struct cdrom_device_info *cdi,
		unsigned long arg)
{
	cd_dbg(CD_DO_IOCTL, "%sabling debug\n", arg ? "En" : "Dis");

	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
	debug = arg ? 1 : 0;
	return debug;
}
