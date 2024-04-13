void hsr_addr_subst_dest(struct hsr_node *node_src, struct sk_buff *skb,
			 struct hsr_port *port)
{
	struct hsr_node *node_dst;

	if (!skb_mac_header_was_set(skb)) {
		WARN_ONCE(1, "%s: Mac header not set\n", __func__);
		return;
	}

	if (!is_unicast_ether_addr(eth_hdr(skb)->h_dest))
		return;

	node_dst = find_node_by_AddrA(&port->hsr->node_db, eth_hdr(skb)->h_dest);
	if (!node_dst) {
		WARN_ONCE(1, "%s: Unknown node\n", __func__);
		return;
	}
	if (port->type != node_dst->AddrB_port)
		return;

	ether_addr_copy(eth_hdr(skb)->h_dest, node_dst->MacAddressB);
}
