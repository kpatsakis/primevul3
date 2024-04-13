u32 kernel_sock_ip_overhead(struct sock *sk)
{
	struct inet_sock *inet;
	struct ip_options_rcu *opt;
	u32 overhead = 0;
#if IS_ENABLED(CONFIG_IPV6)
	struct ipv6_pinfo *np;
	struct ipv6_txoptions *optv6 = NULL;
#endif /* IS_ENABLED(CONFIG_IPV6) */

	if (!sk)
		return overhead;

	switch (sk->sk_family) {
	case AF_INET:
		inet = inet_sk(sk);
		overhead += sizeof(struct iphdr);
		opt = rcu_dereference_protected(inet->inet_opt,
						sock_owned_by_user(sk));
		if (opt)
			overhead += opt->opt.optlen;
		return overhead;
#if IS_ENABLED(CONFIG_IPV6)
	case AF_INET6:
		np = inet6_sk(sk);
		overhead += sizeof(struct ipv6hdr);
		if (np)
			optv6 = rcu_dereference_protected(np->opt,
							  sock_owned_by_user(sk));
		if (optv6)
			overhead += (optv6->opt_flen + optv6->opt_nflen);
		return overhead;
#endif /* IS_ENABLED(CONFIG_IPV6) */
	default: /* Returns 0 overhead if the socket is not ipv4 or ipv6 */
		return overhead;
	}
}
