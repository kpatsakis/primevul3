static int compat_dev_ifconf(struct net *net, struct compat_ifconf __user *uifc32)
{
	struct compat_ifconf ifc32;
	struct ifconf ifc;
	int err;

	if (copy_from_user(&ifc32, uifc32, sizeof(struct compat_ifconf)))
		return -EFAULT;

	ifc.ifc_len = ifc32.ifc_len;
	ifc.ifc_req = compat_ptr(ifc32.ifcbuf);

	rtnl_lock();
	err = dev_ifconf(net, &ifc, sizeof(struct compat_ifreq));
	rtnl_unlock();
	if (err)
		return err;

	ifc32.ifc_len = ifc.ifc_len;
	if (copy_to_user(uifc32, &ifc32, sizeof(struct compat_ifconf)))
		return -EFAULT;

	return 0;
}
