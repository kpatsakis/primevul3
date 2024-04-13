static void cdrom_update_events(struct cdrom_device_info *cdi,
				unsigned int clearing)
{
	unsigned int events;

	events = cdi->ops->check_events(cdi, clearing, CDSL_CURRENT);
	cdi->vfs_events |= events;
	cdi->ioctl_events |= events;
}
