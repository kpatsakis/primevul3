static struct sk_buff *ctrl_build_family_msg(const struct genl_family *family,
					     u32 portid, int seq, u8 cmd)
{
	struct sk_buff *skb;
	int err;

	skb = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (skb == NULL)
		return ERR_PTR(-ENOBUFS);

	err = ctrl_fill_info(family, portid, seq, 0, skb, cmd);
	if (err < 0) {
		nlmsg_free(skb);
		return ERR_PTR(err);
	}

	return skb;
}
