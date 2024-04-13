static void genl_unregister_mc_groups(const struct genl_family *family)
{
	struct net *net;
	int i;

	netlink_table_grab();
	rcu_read_lock();
	for_each_net_rcu(net) {
		for (i = 0; i < family->n_mcgrps; i++)
			__netlink_clear_multicast_users(
				net->genl_sock, family->mcgrp_offset + i);
	}
	rcu_read_unlock();
	netlink_table_ungrab();

	for (i = 0; i < family->n_mcgrps; i++) {
		int grp_id = family->mcgrp_offset + i;

		if (grp_id != 1)
			clear_bit(grp_id, mc_groups);
		genl_ctrl_event(CTRL_CMD_DELMCAST_GRP, family,
				&family->mcgrps[i], grp_id);
	}
}
