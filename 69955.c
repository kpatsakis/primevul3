static void hsr_announce(struct timer_list *t)
{
	struct hsr_priv *hsr;
	struct hsr_port *master;

	hsr = from_timer(hsr, t, announce_timer);

	rcu_read_lock();
	master = hsr_port_get_hsr(hsr, HSR_PT_MASTER);

	if (hsr->announce_count < 3 && hsr->protVersion == 0) {
		send_hsr_supervision_frame(master, HSR_TLV_ANNOUNCE,
				hsr->protVersion);
		hsr->announce_count++;

		hsr->announce_timer.expires = jiffies +
				msecs_to_jiffies(HSR_ANNOUNCE_INTERVAL);
	} else {
		send_hsr_supervision_frame(master, HSR_TLV_LIFE_CHECK,
				hsr->protVersion);

		hsr->announce_timer.expires = jiffies +
				msecs_to_jiffies(HSR_LIFE_CHECK_INTERVAL);
	}

	if (is_admin_up(master->dev))
		add_timer(&hsr->announce_timer);

	rcu_read_unlock();
}
