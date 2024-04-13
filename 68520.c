static int __init arp_tables_init(void)
{
	int ret;

	ret = register_pernet_subsys(&arp_tables_net_ops);
	if (ret < 0)
		goto err1;

	/* No one else will be downing sem now, so we won't sleep */
	ret = xt_register_targets(arpt_builtin_tg, ARRAY_SIZE(arpt_builtin_tg));
	if (ret < 0)
		goto err2;

	/* Register setsockopt */
	ret = nf_register_sockopt(&arpt_sockopts);
	if (ret < 0)
		goto err4;

	return 0;

err4:
	xt_unregister_targets(arpt_builtin_tg, ARRAY_SIZE(arpt_builtin_tg));
err2:
	unregister_pernet_subsys(&arp_tables_net_ops);
err1:
	return ret;
}
