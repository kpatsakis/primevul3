static int __init net_ns_init(void)
{
	struct net_generic *ng;

#ifdef CONFIG_NET_NS
	net_cachep = kmem_cache_create("net_namespace", sizeof(struct net),
					SMP_CACHE_BYTES,
					SLAB_PANIC|SLAB_ACCOUNT, NULL);

	/* Create workqueue for cleanup */
	netns_wq = create_singlethread_workqueue("netns");
	if (!netns_wq)
		panic("Could not create netns workq");
#endif

	ng = net_alloc_generic();
	if (!ng)
		panic("Could not allocate generic netns");

	rcu_assign_pointer(init_net.gen, ng);

	down_write(&pernet_ops_rwsem);
	if (setup_net(&init_net, &init_user_ns))
		panic("Could not setup the initial network namespace");

	init_net_initialized = true;
	up_write(&pernet_ops_rwsem);

	if (register_pernet_subsys(&net_ns_ops))
		panic("Could not register network namespace subsystems");

	rtnl_register(PF_UNSPEC, RTM_NEWNSID, rtnl_net_newid, NULL,
		      RTNL_FLAG_DOIT_UNLOCKED);
	rtnl_register(PF_UNSPEC, RTM_GETNSID, rtnl_net_getid, rtnl_net_dumpid,
		      RTNL_FLAG_DOIT_UNLOCKED);

	return 0;
}
