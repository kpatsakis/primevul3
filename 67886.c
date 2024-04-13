static int mac80211_hwsim_netlink_notify(struct notifier_block *nb,
					 unsigned long state,
					 void *_notify)
{
	struct netlink_notify *notify = _notify;

	if (state != NETLINK_URELEASE)
		return NOTIFY_DONE;

	remove_user_radios(notify->portid);

	if (notify->portid == hwsim_net_get_wmediumd(notify->net)) {
		printk(KERN_INFO "mac80211_hwsim: wmediumd released netlink"
		       " socket, switching to perfect channel medium\n");
		hwsim_register_wmediumd(notify->net, 0);
	}
	return NOTIFY_DONE;

}
