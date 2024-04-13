static int genl_validate_assign_mc_groups(struct genl_family *family)
{
	int first_id;
	int n_groups = family->n_mcgrps;
	int err = 0, i;
	bool groups_allocated = false;

	if (!n_groups)
		return 0;

	for (i = 0; i < n_groups; i++) {
		const struct genl_multicast_group *grp = &family->mcgrps[i];

		if (WARN_ON(grp->name[0] == '\0'))
			return -EINVAL;
		if (WARN_ON(memchr(grp->name, '\0', GENL_NAMSIZ) == NULL))
			return -EINVAL;
	}

	/* special-case our own group and hacks */
	if (family == &genl_ctrl) {
		first_id = GENL_ID_CTRL;
		BUG_ON(n_groups != 1);
	} else if (strcmp(family->name, "NET_DM") == 0) {
		first_id = 1;
		BUG_ON(n_groups != 1);
	} else if (family->id == GENL_ID_VFS_DQUOT) {
		first_id = GENL_ID_VFS_DQUOT;
		BUG_ON(n_groups != 1);
	} else if (family->id == GENL_ID_PMCRAID) {
		first_id = GENL_ID_PMCRAID;
		BUG_ON(n_groups != 1);
	} else {
		groups_allocated = true;
		err = genl_allocate_reserve_groups(n_groups, &first_id);
		if (err)
			return err;
	}

	family->mcgrp_offset = first_id;

	/* if still initializing, can't and don't need to to realloc bitmaps */
	if (!init_net.genl_sock)
		return 0;

	if (family->netnsok) {
		struct net *net;

		netlink_table_grab();
		rcu_read_lock();
		for_each_net_rcu(net) {
			err = __netlink_change_ngroups(net->genl_sock,
					mc_groups_longs * BITS_PER_LONG);
			if (err) {
				/*
				 * No need to roll back, can only fail if
				 * memory allocation fails and then the
				 * number of _possible_ groups has been
				 * increased on some sockets which is ok.
				 */
				break;
			}
		}
		rcu_read_unlock();
		netlink_table_ungrab();
	} else {
		err = netlink_change_ngroups(init_net.genl_sock,
					     mc_groups_longs * BITS_PER_LONG);
	}

	if (groups_allocated && err) {
		for (i = 0; i < family->n_mcgrps; i++)
			clear_bit(family->mcgrp_offset + i, mc_groups);
	}

	return err;
}
