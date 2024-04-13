int hsr_create_self_node(struct list_head *self_node_db,
			 unsigned char addr_a[ETH_ALEN],
			 unsigned char addr_b[ETH_ALEN])
{
	struct hsr_node *node, *oldnode;

	node = kmalloc(sizeof(*node), GFP_KERNEL);
	if (!node)
		return -ENOMEM;

	ether_addr_copy(node->MacAddressA, addr_a);
	ether_addr_copy(node->MacAddressB, addr_b);

	rcu_read_lock();
	oldnode = list_first_or_null_rcu(self_node_db,
						struct hsr_node, mac_list);
	if (oldnode) {
		list_replace_rcu(&oldnode->mac_list, &node->mac_list);
		rcu_read_unlock();
		synchronize_rcu();
		kfree(oldnode);
	} else {
		rcu_read_unlock();
		list_add_tail_rcu(&node->mac_list, self_node_db);
	}

 	return 0;
 }
