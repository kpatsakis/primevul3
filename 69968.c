static bool is_admin_up(struct net_device *dev)
{
	return dev && (dev->flags & IFF_UP);
}
