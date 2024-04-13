static int genl_ctrl_event(int event, const struct genl_family *family,
			   const struct genl_multicast_group *grp,
			   int grp_id)
{
	struct sk_buff *msg;

	/* genl is still initialising */
	if (!init_net.genl_sock)
		return 0;

	switch (event) {
	case CTRL_CMD_NEWFAMILY:
	case CTRL_CMD_DELFAMILY:
		WARN_ON(grp);
		msg = ctrl_build_family_msg(family, 0, 0, event);
		break;
	case CTRL_CMD_NEWMCAST_GRP:
	case CTRL_CMD_DELMCAST_GRP:
		BUG_ON(!grp);
		msg = ctrl_build_mcgrp_msg(family, grp, grp_id, 0, 0, event);
		break;
	default:
		return -EINVAL;
	}

	if (IS_ERR(msg))
		return PTR_ERR(msg);

	if (!family->netnsok) {
		genlmsg_multicast_netns(&genl_ctrl, &init_net, msg, 0,
					0, GFP_KERNEL);
	} else {
		rcu_read_lock();
		genlmsg_multicast_allns(&genl_ctrl, msg, 0,
					0, GFP_ATOMIC);
		rcu_read_unlock();
	}

	return 0;
}
