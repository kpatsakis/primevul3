static int __init hwsim_init_netlink(void)
{
	int rc;

	printk(KERN_INFO "mac80211_hwsim: initializing netlink\n");

	rc = genl_register_family(&hwsim_genl_family);
	if (rc)
		goto failure;

	rc = netlink_register_notifier(&hwsim_netlink_notifier);
	if (rc) {
		genl_unregister_family(&hwsim_genl_family);
		goto failure;
	}

	return 0;

failure:
	pr_debug("mac80211_hwsim: error occurred in %s\n", __func__);
	return -EINVAL;
}
