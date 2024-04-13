static ssize_t tun_show_owner(struct device *dev, struct device_attribute *attr,
			      char *buf)
{
	struct tun_struct *tun = netdev_priv(to_net_dev(dev));
	return uid_valid(tun->owner)?
		sprintf(buf, "%u\n",
			from_kuid_munged(current_user_ns(), tun->owner)):
		sprintf(buf, "-1\n");
}
