static void hns_nic_tx_clr_all_bufs(struct hns_nic_ring_data *ring_data)
{
	struct hnae_ring *ring = ring_data->ring;
	struct net_device *ndev = ring_data->napi.dev;
	struct netdev_queue *dev_queue;
	int head;
	int bytes, pkts;

	NETIF_TX_LOCK(ring);

	head = ring->next_to_use; /* ntu :soft setted ring position*/
	bytes = 0;
	pkts = 0;
	while (head != ring->next_to_clean)
		hns_nic_reclaim_one_desc(ring, &bytes, &pkts);

	NETIF_TX_UNLOCK(ring);

	dev_queue = netdev_get_tx_queue(ndev, ring_data->queue_index);
	netdev_tx_reset_queue(dev_queue);
}
