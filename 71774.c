int peernet2id_alloc(struct net *net, struct net *peer)
{
	bool alloc = false, alive = false;
	int id;

	if (refcount_read(&net->count) == 0)
		return NETNSA_NSID_NOT_ASSIGNED;
	spin_lock_bh(&net->nsid_lock);
	/*
	 * When peer is obtained from RCU lists, we may race with
	 * its cleanup. Check whether it's alive, and this guarantees
	 * we never hash a peer back to net->netns_ids, after it has
	 * just been idr_remove()'d from there in cleanup_net().
	 */
	if (maybe_get_net(peer))
		alive = alloc = true;
	id = __peernet2id_alloc(net, peer, &alloc);
	spin_unlock_bh(&net->nsid_lock);
	if (alloc && id >= 0)
		rtnl_net_notifyid(net, RTM_NEWNSID, id);
	if (alive)
		put_net(peer);
	return id;
}
