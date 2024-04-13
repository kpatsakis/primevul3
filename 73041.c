static void tun_detach_all(struct net_device *dev)
{
	struct tun_struct *tun = netdev_priv(dev);
	struct bpf_prog *xdp_prog = rtnl_dereference(tun->xdp_prog);
	struct tun_file *tfile, *tmp;
	int i, n = tun->numqueues;

	for (i = 0; i < n; i++) {
		tfile = rtnl_dereference(tun->tfiles[i]);
		BUG_ON(!tfile);
		tfile->socket.sk->sk_shutdown = RCV_SHUTDOWN;
		tfile->socket.sk->sk_data_ready(tfile->socket.sk);
		RCU_INIT_POINTER(tfile->tun, NULL);
		--tun->numqueues;
	}
	list_for_each_entry(tfile, &tun->disabled, next) {
		tfile->socket.sk->sk_shutdown = RCV_SHUTDOWN;
		tfile->socket.sk->sk_data_ready(tfile->socket.sk);
		RCU_INIT_POINTER(tfile->tun, NULL);
	}
	BUG_ON(tun->numqueues != 0);

	synchronize_net();
	for (i = 0; i < n; i++) {
		tfile = rtnl_dereference(tun->tfiles[i]);
		/* Drop read queue */
		tun_queue_purge(tfile);
		sock_put(&tfile->sk);
	}
	list_for_each_entry_safe(tfile, tmp, &tun->disabled, next) {
		tun_enable_queue(tfile);
		tun_queue_purge(tfile);
		sock_put(&tfile->sk);
	}
	BUG_ON(tun->numdisabled != 0);

	if (xdp_prog)
		bpf_prog_put(xdp_prog);

	if (tun->flags & IFF_PERSIST)
		module_put(THIS_MODULE);
}
