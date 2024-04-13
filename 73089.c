static long tun_set_vnet_be(struct tun_struct *tun, int __user *argp)
{
	int be;

	if (get_user(be, argp))
		return -EFAULT;

	if (be)
		tun->flags |= TUN_VNET_BE;
	else
		tun->flags &= ~TUN_VNET_BE;

	return 0;
}
