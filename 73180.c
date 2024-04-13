int netdev_txq_to_tc(struct net_device *dev, unsigned int txq)
{
	if (dev->num_tc) {
		struct netdev_tc_txq *tc = &dev->tc_to_txq[0];
		int i;

		for (i = 0; i < TC_MAX_QUEUE; i++, tc++) {
			if ((txq - tc->offset) < tc->count)
				return i;
		}

		return -1;
	}

	return 0;
}
