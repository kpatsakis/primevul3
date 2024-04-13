static int tun_net_close(struct net_device *dev)
{
	netif_tx_stop_all_queues(dev);
	return 0;
}
