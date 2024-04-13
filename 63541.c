static int cdrom_ioctl_get_subchnl(struct cdrom_device_info *cdi,
		void __user *argp)
{
	struct cdrom_subchnl q;
	u8 requested, back;
	int ret;

	/* cd_dbg(CD_DO_IOCTL,"entering CDROMSUBCHNL\n");*/

	if (copy_from_user(&q, argp, sizeof(q)))
		return -EFAULT;

	requested = q.cdsc_format;
	if (requested != CDROM_MSF && requested != CDROM_LBA)
		return -EINVAL;
	q.cdsc_format = CDROM_MSF;

	ret = cdi->ops->audio_ioctl(cdi, CDROMSUBCHNL, &q);
	if (ret)
		return ret;

	back = q.cdsc_format; /* local copy */
	sanitize_format(&q.cdsc_absaddr, &back, requested);
	sanitize_format(&q.cdsc_reladdr, &q.cdsc_format, requested);

	if (copy_to_user(argp, &q, sizeof(q)))
		return -EFAULT;
	/* cd_dbg(CD_DO_IOCTL, "CDROMSUBCHNL successful\n"); */
	return 0;
}
