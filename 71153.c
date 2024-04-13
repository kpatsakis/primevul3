static void rds_tcp_exit(void)
{
	rds_tcp_set_unloading();
	synchronize_rcu();
	rds_info_deregister_func(RDS_INFO_TCP_SOCKETS, rds_tcp_tc_info);
#if IS_ENABLED(CONFIG_IPV6)
	rds_info_deregister_func(RDS6_INFO_TCP_SOCKETS, rds6_tcp_tc_info);
#endif
	unregister_pernet_device(&rds_tcp_net_ops);
	rds_tcp_destroy_conns();
	rds_trans_unregister(&rds_tcp_transport);
	rds_tcp_recv_exit();
	kmem_cache_destroy(rds_tcp_conn_slab);
}
