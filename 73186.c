static int netif_alloc_rx_queues(struct net_device *dev)
{
	unsigned int i, count = dev->num_rx_queues;
	struct netdev_rx_queue *rx;
	size_t sz = count * sizeof(*rx);

	BUG_ON(count < 1);

	rx = kvzalloc(sz, GFP_KERNEL | __GFP_RETRY_MAYFAIL);
	if (!rx)
		return -ENOMEM;

	dev->_rx = rx;

	for (i = 0; i < count; i++)
		rx[i].dev = dev;
	return 0;
}
