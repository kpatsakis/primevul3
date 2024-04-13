int dev_set_alias(struct net_device *dev, const char *alias, size_t len)
{
	char *new_ifalias;

	ASSERT_RTNL();

	if (len >= IFALIASZ)
		return -EINVAL;

	if (!len) {
		kfree(dev->ifalias);
		dev->ifalias = NULL;
		return 0;
	}

	new_ifalias = krealloc(dev->ifalias, len + 1, GFP_KERNEL);
	if (!new_ifalias)
		return -ENOMEM;
	dev->ifalias = new_ifalias;
	memcpy(dev->ifalias, alias, len);
	dev->ifalias[len] = 0;

	return len;
}
