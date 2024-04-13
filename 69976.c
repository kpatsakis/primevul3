void *hsr_get_next_node(struct hsr_priv *hsr, void *_pos,
			unsigned char addr[ETH_ALEN])
{
	struct hsr_node *node;

	if (!_pos) {
		node = list_first_or_null_rcu(&hsr->node_db,
					      struct hsr_node, mac_list);
		if (node)
			ether_addr_copy(addr, node->MacAddressA);
		return node;
	}

	node = _pos;
	list_for_each_entry_continue_rcu(node, &hsr->node_db, mac_list) {
		ether_addr_copy(addr, node->MacAddressA);
		return node;
	}

	return NULL;
}
