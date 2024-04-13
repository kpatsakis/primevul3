static void ipip6_dellink(struct net_device *dev, struct list_head *head)
{
	struct net *net = dev_net(dev);
	struct sit_net *sitn = net_generic(net, sit_net_id);

	if (dev != sitn->fb_tunnel_dev)
		unregister_netdevice_queue(dev, head);
}
