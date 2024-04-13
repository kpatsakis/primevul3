static void __tun_detach(struct tun_file *tfile, bool clean)
{
	struct tun_file *ntfile;
	struct tun_struct *tun;

	tun = rtnl_dereference(tfile->tun);

	if (tun && !tfile->detached) {
		u16 index = tfile->queue_index;
		BUG_ON(index >= tun->numqueues);

		rcu_assign_pointer(tun->tfiles[index],
				   tun->tfiles[tun->numqueues - 1]);
		ntfile = rtnl_dereference(tun->tfiles[index]);
		ntfile->queue_index = index;

		--tun->numqueues;
		if (clean) {
			RCU_INIT_POINTER(tfile->tun, NULL);
			sock_put(&tfile->sk);
		} else
			tun_disable_queue(tun, tfile);

		synchronize_net();
		tun_flow_delete_by_queue(tun, tun->numqueues + 1);
		/* Drop read queue */
		tun_queue_purge(tfile);
		tun_set_real_num_queues(tun);
	} else if (tfile->detached && clean) {
		tun = tun_enable_queue(tfile);
		sock_put(&tfile->sk);
	}

	if (clean) {
		if (tun && tun->numqueues == 0 && tun->numdisabled == 0) {
			netif_carrier_off(tun->dev);

			if (!(tun->flags & IFF_PERSIST) &&
			    tun->dev->reg_state == NETREG_REGISTERED)
				unregister_netdevice(tun->dev);
		}
		if (tun)
			skb_array_cleanup(&tfile->tx_array);
		sock_put(&tfile->sk);
	}
}
