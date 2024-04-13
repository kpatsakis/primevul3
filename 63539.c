static int cdrom_ioctl_get_capability(struct cdrom_device_info *cdi)
{
	cd_dbg(CD_DO_IOCTL, "entering CDROM_GET_CAPABILITY\n");
	return (cdi->ops->capability & ~cdi->mask);
}
