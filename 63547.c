static int cdrom_ioctl_reset(struct cdrom_device_info *cdi,
		struct block_device *bdev)
{
	cd_dbg(CD_DO_IOCTL, "entering CDROM_RESET\n");

	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
	if (!CDROM_CAN(CDC_RESET))
		return -ENOSYS;
	invalidate_bdev(bdev);
	return cdi->ops->reset(cdi);
}
