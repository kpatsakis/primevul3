static long tun_get_vnet_be(struct tun_struct *tun, int __user *argp)
{
	int be = !!(tun->flags & TUN_VNET_BE);

	if (put_user(be, argp))
		return -EFAULT;

	return 0;
}
