static int cdrom_ioctl_read_tocentry(struct cdrom_device_info *cdi,
		void __user *argp)
{
	struct cdrom_tocentry entry;
	u8 requested_format;
	int ret;

	/* cd_dbg(CD_DO_IOCTL, "entering CDROMREADTOCENTRY\n"); */

	if (copy_from_user(&entry, argp, sizeof(entry)))
		return -EFAULT;

	requested_format = entry.cdte_format;
	if (requested_format != CDROM_MSF && requested_format != CDROM_LBA)
		return -EINVAL;
	/* make interface to low-level uniform */
	entry.cdte_format = CDROM_MSF;
	ret = cdi->ops->audio_ioctl(cdi, CDROMREADTOCENTRY, &entry);
	if (ret)
		return ret;
	sanitize_format(&entry.cdte_addr, &entry.cdte_format, requested_format);

	if (copy_to_user(argp, &entry, sizeof(entry)))
		return -EFAULT;
	/* cd_dbg(CD_DO_IOCTL, "CDROMREADTOCENTRY successful\n"); */
	return 0;
}
