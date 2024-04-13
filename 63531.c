static int cdrom_ioctl_changer_nslots(struct cdrom_device_info *cdi)
{
	cd_dbg(CD_DO_IOCTL, "entering CDROM_CHANGER_NSLOTS\n");
	return cdi->capacity;
}
