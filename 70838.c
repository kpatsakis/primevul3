void nfc_genl_exit(void)
{
	netlink_unregister_notifier(&nl_notifier);
	genl_unregister_family(&nfc_genl_family);
}
