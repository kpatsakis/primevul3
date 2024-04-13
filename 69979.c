void hsr_handle_sup_frame(struct sk_buff *skb, struct hsr_node *node_curr,
			  struct hsr_port *port_rcv)
{
	struct ethhdr *ethhdr;
	struct hsr_node *node_real;
	struct hsr_sup_payload *hsr_sp;
	struct list_head *node_db;
	int i;

	ethhdr = (struct ethhdr *) skb_mac_header(skb);

	/* Leave the ethernet header. */
	skb_pull(skb, sizeof(struct ethhdr));

	/* And leave the HSR tag. */
	if (ethhdr->h_proto == htons(ETH_P_HSR))
		skb_pull(skb, sizeof(struct hsr_tag));

	/* And leave the HSR sup tag. */
	skb_pull(skb, sizeof(struct hsr_sup_tag));

	hsr_sp = (struct hsr_sup_payload *) skb->data;

	/* Merge node_curr (registered on MacAddressB) into node_real */
	node_db = &port_rcv->hsr->node_db;
	node_real = find_node_by_AddrA(node_db, hsr_sp->MacAddressA);
	if (!node_real)
		/* No frame received from AddrA of this node yet */
		node_real = hsr_add_node(node_db, hsr_sp->MacAddressA,
					 HSR_SEQNR_START - 1);
	if (!node_real)
		goto done; /* No mem */
	if (node_real == node_curr)
		/* Node has already been merged */
		goto done;

	ether_addr_copy(node_real->MacAddressB, ethhdr->h_source);
	for (i = 0; i < HSR_PT_PORTS; i++) {
		if (!node_curr->time_in_stale[i] &&
		    time_after(node_curr->time_in[i], node_real->time_in[i])) {
			node_real->time_in[i] = node_curr->time_in[i];
			node_real->time_in_stale[i] = node_curr->time_in_stale[i];
		}
		if (seq_nr_after(node_curr->seq_out[i], node_real->seq_out[i]))
			node_real->seq_out[i] = node_curr->seq_out[i];
	}
	node_real->AddrB_port = port_rcv->type;

	list_del_rcu(&node_curr->mac_list);
	kfree_rcu(node_curr, rcu_head);

done:
	skb_push(skb, sizeof(struct hsrv1_ethhdr_sp));
}
