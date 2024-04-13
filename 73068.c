static int __init tun_init(void)
{
	int ret = 0;

	pr_info("%s, %s\n", DRV_DESCRIPTION, DRV_VERSION);

	ret = rtnl_link_register(&tun_link_ops);
	if (ret) {
		pr_err("Can't register link_ops\n");
		goto err_linkops;
	}

	ret = misc_register(&tun_miscdev);
	if (ret) {
		pr_err("Can't register misc device %d\n", TUN_MINOR);
		goto err_misc;
	}

	ret = register_netdevice_notifier(&tun_notifier_block);
	if (ret) {
		pr_err("Can't register netdevice notifier\n");
		goto err_notifier;
	}

	return  0;

err_notifier:
	misc_deregister(&tun_miscdev);
err_misc:
	rtnl_link_unregister(&tun_link_ops);
err_linkops:
	return ret;
}
