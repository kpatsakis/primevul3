int __init hidp_init_sockets(void)
{
	int err;

	err = proto_register(&hidp_proto, 0);
	if (err < 0)
		return err;

	err = bt_sock_register(BTPROTO_HIDP, &hidp_sock_family_ops);
	if (err < 0) {
		BT_ERR("Can't register HIDP socket");
		goto error;
	}

	err = bt_procfs_init(&init_net, "hidp", &hidp_sk_list, NULL);
	if (err < 0) {
		BT_ERR("Failed to create HIDP proc file");
		bt_sock_unregister(BTPROTO_HIDP);
		goto error;
	}

	BT_INFO("HIDP socket layer initialized");

	return 0;

error:
	proto_unregister(&hidp_proto);
	return err;
}
