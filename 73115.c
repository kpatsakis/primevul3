static int __netdev_upper_dev_link(struct net_device *dev,
				   struct net_device *upper_dev, bool master,
				   void *upper_priv, void *upper_info)
{
	struct netdev_notifier_changeupper_info changeupper_info;
	int ret = 0;

	ASSERT_RTNL();

	if (dev == upper_dev)
		return -EBUSY;

	/* To prevent loops, check if dev is not upper device to upper_dev. */
	if (netdev_has_upper_dev(upper_dev, dev))
		return -EBUSY;

	if (netdev_has_upper_dev(dev, upper_dev))
		return -EEXIST;

	if (master && netdev_master_upper_dev_get(dev))
		return -EBUSY;

	changeupper_info.upper_dev = upper_dev;
	changeupper_info.master = master;
	changeupper_info.linking = true;
	changeupper_info.upper_info = upper_info;

	ret = call_netdevice_notifiers_info(NETDEV_PRECHANGEUPPER, dev,
					    &changeupper_info.info);
	ret = notifier_to_errno(ret);
	if (ret)
		return ret;

	ret = __netdev_adjacent_dev_link_neighbour(dev, upper_dev, upper_priv,
						   master);
	if (ret)
		return ret;

	ret = call_netdevice_notifiers_info(NETDEV_CHANGEUPPER, dev,
					    &changeupper_info.info);
	ret = notifier_to_errno(ret);
	if (ret)
		goto rollback;

	return 0;

rollback:
	__netdev_adjacent_dev_unlink_neighbour(dev, upper_dev);

	return ret;
}
