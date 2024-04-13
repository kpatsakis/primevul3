int dev_set_mtu(struct net_device *dev, int new_mtu)
{
	int err, orig_mtu;

	if (new_mtu == dev->mtu)
		return 0;

	/* MTU must be positive, and in range */
	if (new_mtu < 0 || new_mtu < dev->min_mtu) {
		net_err_ratelimited("%s: Invalid MTU %d requested, hw min %d\n",
				    dev->name, new_mtu, dev->min_mtu);
		return -EINVAL;
	}

	if (dev->max_mtu > 0 && new_mtu > dev->max_mtu) {
		net_err_ratelimited("%s: Invalid MTU %d requested, hw max %d\n",
				    dev->name, new_mtu, dev->max_mtu);
		return -EINVAL;
	}

	if (!netif_device_present(dev))
		return -ENODEV;

	err = call_netdevice_notifiers(NETDEV_PRECHANGEMTU, dev);
	err = notifier_to_errno(err);
	if (err)
		return err;

	orig_mtu = dev->mtu;
	err = __dev_set_mtu(dev, new_mtu);

	if (!err) {
		err = call_netdevice_notifiers(NETDEV_CHANGEMTU, dev);
		err = notifier_to_errno(err);
		if (err) {
			/* setting mtu back and notifying everyone again,
			 * so that they have a chance to revert changes.
			 */
			__dev_set_mtu(dev, orig_mtu);
			call_netdevice_notifiers(NETDEV_CHANGEMTU, dev);
		}
	}
	return err;
}
