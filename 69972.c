static struct hsr_node *find_node_by_AddrA(struct list_head *node_db,
					   const unsigned char addr[ETH_ALEN])
{
	struct hsr_node *node;

	list_for_each_entry_rcu(node, node_db, mac_list) {
		if (ether_addr_equal(node->MacAddressA, addr))
			return node;
	}

	return NULL;
}
