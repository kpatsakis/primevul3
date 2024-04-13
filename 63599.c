static noinline int mmc_ioctl_dvd_read_struct(struct cdrom_device_info *cdi,
					      void __user *arg,
					      struct packet_command *cgc)
{
	int ret;
	dvd_struct *s;
	int size = sizeof(dvd_struct);

	if (!CDROM_CAN(CDC_DVD))
		return -ENOSYS;

	s = memdup_user(arg, size);
	if (IS_ERR(s))
		return PTR_ERR(s);

	cd_dbg(CD_DO_IOCTL, "entering DVD_READ_STRUCT\n");

	ret = dvd_read_struct(cdi, s, cgc);
	if (ret)
		goto out;

	if (copy_to_user(arg, s, size))
		ret = -EFAULT;
out:
	kfree(s);
	return ret;
}
