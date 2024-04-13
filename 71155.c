static __net_init int rds_tcp_init_net(struct net *net)
{
	struct rds_tcp_net *rtn = net_generic(net, rds_tcp_netid);
	struct ctl_table *tbl;
	int err = 0;

	memset(rtn, 0, sizeof(*rtn));

	/* {snd, rcv}buf_size default to 0, which implies we let the
	 * stack pick the value, and permit auto-tuning of buffer size.
	 */
	if (net == &init_net) {
		tbl = rds_tcp_sysctl_table;
	} else {
		tbl = kmemdup(rds_tcp_sysctl_table,
			      sizeof(rds_tcp_sysctl_table), GFP_KERNEL);
		if (!tbl) {
			pr_warn("could not set allocate syctl table\n");
			return -ENOMEM;
		}
		rtn->ctl_table = tbl;
	}
	tbl[RDS_TCP_SNDBUF].data = &rtn->sndbuf_size;
	tbl[RDS_TCP_RCVBUF].data = &rtn->rcvbuf_size;
	rtn->rds_tcp_sysctl = register_net_sysctl(net, "net/rds/tcp", tbl);
	if (!rtn->rds_tcp_sysctl) {
		pr_warn("could not register sysctl\n");
		err = -ENOMEM;
		goto fail;
	}

#if IS_ENABLED(CONFIG_IPV6)
	rtn->rds_tcp_listen_sock = rds_tcp_listen_init(net, true);
#else
	rtn->rds_tcp_listen_sock = rds_tcp_listen_init(net, false);
#endif
	if (!rtn->rds_tcp_listen_sock) {
		pr_warn("could not set up IPv6 listen sock\n");

#if IS_ENABLED(CONFIG_IPV6)
		/* Try IPv4 as some systems disable IPv6 */
		rtn->rds_tcp_listen_sock = rds_tcp_listen_init(net, false);
		if (!rtn->rds_tcp_listen_sock) {
#endif
			unregister_net_sysctl_table(rtn->rds_tcp_sysctl);
			rtn->rds_tcp_sysctl = NULL;
			err = -EAFNOSUPPORT;
			goto fail;
#if IS_ENABLED(CONFIG_IPV6)
		}
#endif
	}
	INIT_WORK(&rtn->rds_tcp_accept_w, rds_tcp_accept_worker);
	return 0;

fail:
	if (net != &init_net)
		kfree(tbl);
	return err;
}
