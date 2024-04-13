static void tun_set_real_num_queues(struct tun_struct *tun)
{
	netif_set_real_num_tx_queues(tun->dev, tun->numqueues);
	netif_set_real_num_rx_queues(tun->dev, tun->numqueues);
}
