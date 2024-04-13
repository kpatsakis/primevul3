struct net *get_net_ns_by_id(struct net *net, int id)
{
	struct net *peer;

	if (id < 0)
		return NULL;

	rcu_read_lock();
	peer = idr_find(&net->netns_ids, id);
	if (peer)
		peer = maybe_get_net(peer);
	rcu_read_unlock();

	return peer;
}
