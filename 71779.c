static int rtnl_net_dumpid_one(int id, void *peer, void *data)
{
	struct rtnl_net_dump_cb *net_cb = (struct rtnl_net_dump_cb *)data;
	int ret;

	if (net_cb->idx < net_cb->s_idx)
		goto cont;

	net_cb->fillargs.nsid = id;
	if (net_cb->fillargs.add_ref)
		net_cb->fillargs.ref_nsid = __peernet2id(net_cb->ref_net, peer);
	ret = rtnl_net_fill(net_cb->skb, &net_cb->fillargs);
	if (ret < 0)
		return ret;

cont:
	net_cb->idx++;
	return 0;
}
