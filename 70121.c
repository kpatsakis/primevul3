int genlmsg_multicast_allns(const struct genl_family *family,
			    struct sk_buff *skb, u32 portid,
			    unsigned int group, gfp_t flags)
{
	if (WARN_ON_ONCE(group >= family->n_mcgrps))
		return -EINVAL;
	group = family->mcgrp_offset + group;
	return genlmsg_mcast(skb, portid, group, flags);
}
