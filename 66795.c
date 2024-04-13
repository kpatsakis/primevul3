static int compat_siocwandev(struct net *net, struct compat_ifreq __user *uifr32)
{
	compat_uptr_t uptr32;
	struct ifreq ifr;
	void __user *saved;
	int err;

	if (copy_from_user(&ifr, uifr32, sizeof(struct compat_ifreq)))
		return -EFAULT;

	if (get_user(uptr32, &uifr32->ifr_settings.ifs_ifsu))
		return -EFAULT;

	saved = ifr.ifr_settings.ifs_ifsu.raw_hdlc;
	ifr.ifr_settings.ifs_ifsu.raw_hdlc = compat_ptr(uptr32);

	err = dev_ioctl(net, SIOCWANDEV, &ifr, NULL);
	if (!err) {
		ifr.ifr_settings.ifs_ifsu.raw_hdlc = saved;
		if (copy_to_user(uifr32, &ifr, sizeof(struct compat_ifreq)))
			err = -EFAULT;
	}
	return err;
}
