static int ctrl_fill_info(const struct genl_family *family, u32 portid, u32 seq,
			  u32 flags, struct sk_buff *skb, u8 cmd)
{
	void *hdr;

	hdr = genlmsg_put(skb, portid, seq, &genl_ctrl, flags, cmd);
	if (hdr == NULL)
		return -1;

	if (nla_put_string(skb, CTRL_ATTR_FAMILY_NAME, family->name) ||
	    nla_put_u16(skb, CTRL_ATTR_FAMILY_ID, family->id) ||
	    nla_put_u32(skb, CTRL_ATTR_VERSION, family->version) ||
	    nla_put_u32(skb, CTRL_ATTR_HDRSIZE, family->hdrsize) ||
	    nla_put_u32(skb, CTRL_ATTR_MAXATTR, family->maxattr))
		goto nla_put_failure;

	if (family->n_ops) {
		struct nlattr *nla_ops;
		int i;

		nla_ops = nla_nest_start(skb, CTRL_ATTR_OPS);
		if (nla_ops == NULL)
			goto nla_put_failure;

		for (i = 0; i < family->n_ops; i++) {
			struct nlattr *nest;
			const struct genl_ops *ops = &family->ops[i];
			u32 op_flags = ops->flags;

			if (ops->dumpit)
				op_flags |= GENL_CMD_CAP_DUMP;
			if (ops->doit)
				op_flags |= GENL_CMD_CAP_DO;
			if (ops->policy)
				op_flags |= GENL_CMD_CAP_HASPOL;

			nest = nla_nest_start(skb, i + 1);
			if (nest == NULL)
				goto nla_put_failure;

			if (nla_put_u32(skb, CTRL_ATTR_OP_ID, ops->cmd) ||
			    nla_put_u32(skb, CTRL_ATTR_OP_FLAGS, op_flags))
				goto nla_put_failure;

			nla_nest_end(skb, nest);
		}

		nla_nest_end(skb, nla_ops);
	}

	if (family->n_mcgrps) {
		struct nlattr *nla_grps;
		int i;

		nla_grps = nla_nest_start(skb, CTRL_ATTR_MCAST_GROUPS);
		if (nla_grps == NULL)
			goto nla_put_failure;

		for (i = 0; i < family->n_mcgrps; i++) {
			struct nlattr *nest;
			const struct genl_multicast_group *grp;

			grp = &family->mcgrps[i];

			nest = nla_nest_start(skb, i + 1);
			if (nest == NULL)
				goto nla_put_failure;

			if (nla_put_u32(skb, CTRL_ATTR_MCAST_GRP_ID,
					family->mcgrp_offset + i) ||
			    nla_put_string(skb, CTRL_ATTR_MCAST_GRP_NAME,
					   grp->name))
				goto nla_put_failure;

			nla_nest_end(skb, nest);
		}
		nla_nest_end(skb, nla_grps);
	}

	genlmsg_end(skb, hdr);
	return 0;

nla_put_failure:
	genlmsg_cancel(skb, hdr);
	return -EMSGSIZE;
}
