static bool is_slave_up(struct net_device *dev)
{
	return dev && is_admin_up(dev) && netif_oper_up(dev);
}
