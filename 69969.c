inline bool is_hsr_master(struct net_device *dev)
{
	return (dev->netdev_ops->ndo_start_xmit == hsr_dev_xmit);
}
