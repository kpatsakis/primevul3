static int tun_net_open(struct net_device *dev)
{
	struct tun_struct *tun = netdev_priv(dev);
	int i;

	netif_tx_start_all_queues(dev);

	for (i = 0; i < tun->numqueues; i++) {
		struct tun_file *tfile;

		tfile = rtnl_dereference(tun->tfiles[i]);
		tfile->socket.sk->sk_write_space(tfile->socket.sk);
	}

	return 0;
}
