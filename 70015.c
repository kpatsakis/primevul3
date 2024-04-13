static void __net_exit sit_exit_batch_net(struct list_head *net_list)
{
	LIST_HEAD(list);
	struct net *net;

	rtnl_lock();
	list_for_each_entry(net, net_list, exit_list)
		sit_destroy_tunnels(net, &list);

	unregister_netdevice_many(&list);
	rtnl_unlock();
}
