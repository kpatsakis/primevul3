struct hsr_node *hsr_get_node(struct hsr_port *port, struct sk_buff *skb,
			      bool is_sup)
{
	struct list_head *node_db = &port->hsr->node_db;
	struct hsr_node *node;
	struct ethhdr *ethhdr;
	u16 seq_out;

	if (!skb_mac_header_was_set(skb))
		return NULL;

	ethhdr = (struct ethhdr *) skb_mac_header(skb);

	list_for_each_entry_rcu(node, node_db, mac_list) {
		if (ether_addr_equal(node->MacAddressA, ethhdr->h_source))
			return node;
		if (ether_addr_equal(node->MacAddressB, ethhdr->h_source))
			return node;
	}

	/* Everyone may create a node entry, connected node to a HSR device. */

	if (ethhdr->h_proto == htons(ETH_P_PRP)
			|| ethhdr->h_proto == htons(ETH_P_HSR)) {
		/* Use the existing sequence_nr from the tag as starting point
		 * for filtering duplicate frames.
		 */
		seq_out = hsr_get_skb_sequence_nr(skb) - 1;
	} else {
		/* this is called also for frames from master port and
		 * so warn only for non master ports
		 */
		if (port->type != HSR_PT_MASTER)
			WARN_ONCE(1, "%s: Non-HSR frame\n", __func__);
		seq_out = HSR_SEQNR_START;
	}

	return hsr_add_node(node_db, ethhdr->h_source, seq_out);
}
