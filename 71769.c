static void cleanup_net(struct work_struct *work)
{
	const struct pernet_operations *ops;
	struct net *net, *tmp, *last;
	struct llist_node *net_kill_list;
	LIST_HEAD(net_exit_list);

	/* Atomically snapshot the list of namespaces to cleanup */
	net_kill_list = llist_del_all(&cleanup_list);

	down_read(&pernet_ops_rwsem);

	/* Don't let anyone else find us. */
	down_write(&net_rwsem);
	llist_for_each_entry(net, net_kill_list, cleanup_list)
		list_del_rcu(&net->list);
	/* Cache last net. After we unlock rtnl, no one new net
	 * added to net_namespace_list can assign nsid pointer
	 * to a net from net_kill_list (see peernet2id_alloc()).
	 * So, we skip them in unhash_nsid().
	 *
	 * Note, that unhash_nsid() does not delete nsid links
	 * between net_kill_list's nets, as they've already
	 * deleted from net_namespace_list. But, this would be
	 * useless anyway, as netns_ids are destroyed there.
	 */
	last = list_last_entry(&net_namespace_list, struct net, list);
	up_write(&net_rwsem);

	llist_for_each_entry(net, net_kill_list, cleanup_list) {
		unhash_nsid(net, last);
		list_add_tail(&net->exit_list, &net_exit_list);
	}

	/*
	 * Another CPU might be rcu-iterating the list, wait for it.
	 * This needs to be before calling the exit() notifiers, so
	 * the rcu_barrier() below isn't sufficient alone.
	 */
	synchronize_rcu();

	/* Run all of the network namespace exit methods */
	list_for_each_entry_reverse(ops, &pernet_list, list)
		ops_exit_list(ops, &net_exit_list);

	/* Free the net generic variables */
	list_for_each_entry_reverse(ops, &pernet_list, list)
		ops_free_list(ops, &net_exit_list);

	up_read(&pernet_ops_rwsem);

	/* Ensure there are no outstanding rcu callbacks using this
	 * network namespace.
	 */
	rcu_barrier();

	/* Finally it is safe to free my network namespace structure */
	list_for_each_entry_safe(net, tmp, &net_exit_list, exit_list) {
		list_del_init(&net->exit_list);
		dec_net_namespaces(net->ucounts);
		put_user_ns(net->user_ns);
		net_drop_ns(net);
	}
}
