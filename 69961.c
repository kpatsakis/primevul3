static void hsr_dev_destroy(struct net_device *hsr_dev)
{
	struct hsr_priv *hsr;
	struct hsr_port *port;

	hsr = netdev_priv(hsr_dev);

	rtnl_lock();
	hsr_for_each_port(hsr, port)
		hsr_del_port(port);
	rtnl_unlock();

	del_timer_sync(&hsr->prune_timer);
	del_timer_sync(&hsr->announce_timer);

	synchronize_rcu();
}
