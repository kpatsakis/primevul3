void __exit hidp_cleanup_sockets(void)
{
	bt_procfs_cleanup(&init_net, "hidp");
	bt_sock_unregister(BTPROTO_HIDP);
	proto_unregister(&hidp_proto);
}
