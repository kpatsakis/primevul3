static int genlmsg_mcast(struct sk_buff *skb, u32 portid, unsigned long group,
			 gfp_t flags)
{
	struct sk_buff *tmp;
	struct net *net, *prev = NULL;
	bool delivered = false;
	int err;

	for_each_net_rcu(net) {
		if (prev) {
			tmp = skb_clone(skb, flags);
			if (!tmp) {
				err = -ENOMEM;
				goto error;
			}
			err = nlmsg_multicast(prev->genl_sock, tmp,
					      portid, group, flags);
			if (!err)
				delivered = true;
			else if (err != -ESRCH)
				goto error;
		}

		prev = net;
	}

	err = nlmsg_multicast(prev->genl_sock, skb, portid, group, flags);
	if (!err)
		delivered = true;
	else if (err != -ESRCH)
		return err;
	return delivered ? 0 : -ESRCH;
 error:
	kfree_skb(skb);
	return err;
}
