static int rds_tcp_laddr_check(struct net *net, const struct in6_addr *addr,
			       __u32 scope_id)
{
	struct net_device *dev = NULL;
#if IS_ENABLED(CONFIG_IPV6)
	int ret;
#endif

	if (ipv6_addr_v4mapped(addr)) {
		if (inet_addr_type(net, addr->s6_addr32[3]) == RTN_LOCAL)
			return 0;
		return -EADDRNOTAVAIL;
	}

	/* If the scope_id is specified, check only those addresses
	 * hosted on the specified interface.
	 */
	if (scope_id != 0) {
		rcu_read_lock();
		dev = dev_get_by_index_rcu(net, scope_id);
		/* scope_id is not valid... */
		if (!dev) {
			rcu_read_unlock();
			return -EADDRNOTAVAIL;
		}
		rcu_read_unlock();
	}
#if IS_ENABLED(CONFIG_IPV6)
	ret = ipv6_chk_addr(net, addr, dev, 0);
	if (ret)
		return 0;
#endif
	return -EADDRNOTAVAIL;
}
