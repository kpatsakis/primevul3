static long sock_do_ioctl(struct net *net, struct socket *sock,
				 unsigned int cmd, unsigned long arg)
{
	int err;
	void __user *argp = (void __user *)arg;

	err = sock->ops->ioctl(sock, cmd, arg);

	/*
	 * If this ioctl is unknown try to hand it down
	 * to the NIC driver.
	 */
	if (err != -ENOIOCTLCMD)
		return err;

	if (cmd == SIOCGIFCONF) {
		struct ifconf ifc;
		if (copy_from_user(&ifc, argp, sizeof(struct ifconf)))
			return -EFAULT;
		rtnl_lock();
		err = dev_ifconf(net, &ifc, sizeof(struct ifreq));
		rtnl_unlock();
		if (!err && copy_to_user(argp, &ifc, sizeof(struct ifconf)))
			err = -EFAULT;
	} else {
		struct ifreq ifr;
		bool need_copyout;
		if (copy_from_user(&ifr, argp, sizeof(struct ifreq)))
			return -EFAULT;
		err = dev_ioctl(net, cmd, &ifr, &need_copyout);
		if (!err && need_copyout)
			if (copy_to_user(argp, &ifr, sizeof(struct ifreq)))
				return -EFAULT;
	}
	return err;
}
