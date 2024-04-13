static void netif_reset_xps_queues(struct net_device *dev, u16 offset,
				   u16 count)
{
	struct xps_dev_maps *dev_maps;
	int cpu, i;
	bool active = false;

	mutex_lock(&xps_map_mutex);
	dev_maps = xmap_dereference(dev->xps_maps);

	if (!dev_maps)
		goto out_no_maps;

	for_each_possible_cpu(cpu)
		active |= remove_xps_queue_cpu(dev, dev_maps, cpu,
					       offset, count);

	if (!active) {
		RCU_INIT_POINTER(dev->xps_maps, NULL);
		kfree_rcu(dev_maps, rcu);
	}

	for (i = offset + (count - 1); count--; i--)
		netdev_queue_numa_node_write(netdev_get_tx_queue(dev, i),
					     NUMA_NO_NODE);

out_no_maps:
	mutex_unlock(&xps_map_mutex);
}
