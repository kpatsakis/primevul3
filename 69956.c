static void hsr_check_announce(struct net_device *hsr_dev,
			       unsigned char old_operstate)
{
	struct hsr_priv *hsr;

	hsr = netdev_priv(hsr_dev);

	if ((hsr_dev->operstate == IF_OPER_UP)
			&& (old_operstate != IF_OPER_UP)) {
		/* Went up */
		hsr->announce_count = 0;
		hsr->announce_timer.expires = jiffies +
				msecs_to_jiffies(HSR_ANNOUNCE_INTERVAL);
		add_timer(&hsr->announce_timer);
	}

	if ((hsr_dev->operstate != IF_OPER_UP) && (old_operstate == IF_OPER_UP))
		/* Went down */
		del_timer(&hsr->announce_timer);
}
