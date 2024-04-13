struct net *copy_net_ns(unsigned long flags,
			struct user_namespace *user_ns, struct net *old_net)
{
	struct ucounts *ucounts;
	struct net *net;
	int rv;

	if (!(flags & CLONE_NEWNET))
		return get_net(old_net);

	ucounts = inc_net_namespaces(user_ns);
	if (!ucounts)
		return ERR_PTR(-ENOSPC);

	net = net_alloc();
	if (!net) {
		rv = -ENOMEM;
		goto dec_ucounts;
	}
	refcount_set(&net->passive, 1);
	net->ucounts = ucounts;
	get_user_ns(user_ns);

	rv = down_read_killable(&pernet_ops_rwsem);
	if (rv < 0)
		goto put_userns;

	rv = setup_net(net, user_ns);

	up_read(&pernet_ops_rwsem);

	if (rv < 0) {
put_userns:
		put_user_ns(user_ns);
		net_drop_ns(net);
dec_ucounts:
		dec_net_namespaces(ucounts);
		return ERR_PTR(rv);
	}
	return net;
}
