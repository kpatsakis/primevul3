static void __exit exit_mac80211_hwsim(void)
{
	pr_debug("mac80211_hwsim: unregister radios\n");

	hwsim_exit_netlink();

	mac80211_hwsim_free();
	rhashtable_destroy(&hwsim_radios_rht);
	unregister_netdev(hwsim_mon);
	platform_driver_unregister(&mac80211_hwsim_driver);
	unregister_pernet_device(&hwsim_net_ops);
}
