static int compat_ifr_data_ioctl(struct net *net, unsigned int cmd,
				 struct compat_ifreq __user *u_ifreq32)
{
	struct ifreq ifreq;
	u32 data32;

	if (copy_from_user(ifreq.ifr_name, u_ifreq32->ifr_name, IFNAMSIZ))
		return -EFAULT;
	if (get_user(data32, &u_ifreq32->ifr_data))
		return -EFAULT;
	ifreq.ifr_data = compat_ptr(data32);

	return dev_ioctl(net, cmd, &ifreq, NULL);
}
