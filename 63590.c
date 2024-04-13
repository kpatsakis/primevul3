int media_changed(struct cdrom_device_info *cdi, int queue)
{
	unsigned int mask = (1 << (queue & 1));
	int ret = !!(cdi->mc_flags & mask);
	bool changed;

	if (!CDROM_CAN(CDC_MEDIA_CHANGED))
		return ret;

	/* changed since last call? */
	if (cdi->ops->check_events) {
		BUG_ON(!queue);	/* shouldn't be called from VFS path */
		cdrom_update_events(cdi, DISK_EVENT_MEDIA_CHANGE);
		changed = cdi->ioctl_events & DISK_EVENT_MEDIA_CHANGE;
		cdi->ioctl_events = 0;
	} else
		changed = cdi->ops->media_changed(cdi, CDSL_CURRENT);

	if (changed) {
		cdi->mc_flags = 0x3;    /* set bit on both queues */
		ret |= 1;
		cdi->media_written = 0;
	}

	cdi->mc_flags &= ~mask;         /* clear bit */
	return ret;
}
