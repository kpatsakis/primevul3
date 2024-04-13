bool hsr_addr_is_self(struct hsr_priv *hsr, unsigned char *addr)
{
	struct hsr_node *node;

	node = list_first_or_null_rcu(&hsr->self_node_db, struct hsr_node,
				      mac_list);
	if (!node) {
		WARN_ONCE(1, "HSR: No self node\n");
		return false;
	}

	if (ether_addr_equal(addr, node->MacAddressA))
		return true;
	if (ether_addr_equal(addr, node->MacAddressB))
		return true;

	return false;
}
