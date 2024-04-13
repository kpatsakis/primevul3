static void tun_flow_init(struct tun_struct *tun)
{
	int i;

	for (i = 0; i < TUN_NUM_FLOW_ENTRIES; i++)
		INIT_HLIST_HEAD(&tun->flows[i]);

	tun->ageing_time = TUN_FLOW_EXPIRE;
	setup_timer(&tun->flow_gc_timer, tun_flow_cleanup, (unsigned long)tun);
	mod_timer(&tun->flow_gc_timer,
		  round_jiffies_up(jiffies + tun->ageing_time));
}
