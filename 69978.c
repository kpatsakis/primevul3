int hsr_get_node_data(struct hsr_priv *hsr,
		      const unsigned char *addr,
		      unsigned char addr_b[ETH_ALEN],
		      unsigned int *addr_b_ifindex,
		      int *if1_age,
		      u16 *if1_seq,
		      int *if2_age,
		      u16 *if2_seq)
{
	struct hsr_node *node;
	struct hsr_port *port;
	unsigned long tdiff;


	rcu_read_lock();
	node = find_node_by_AddrA(&hsr->node_db, addr);
	if (!node) {
		rcu_read_unlock();
		return -ENOENT;	/* No such entry */
	}

	ether_addr_copy(addr_b, node->MacAddressB);

	tdiff = jiffies - node->time_in[HSR_PT_SLAVE_A];
	if (node->time_in_stale[HSR_PT_SLAVE_A])
		*if1_age = INT_MAX;
#if HZ <= MSEC_PER_SEC
	else if (tdiff > msecs_to_jiffies(INT_MAX))
		*if1_age = INT_MAX;
#endif
	else
		*if1_age = jiffies_to_msecs(tdiff);

	tdiff = jiffies - node->time_in[HSR_PT_SLAVE_B];
	if (node->time_in_stale[HSR_PT_SLAVE_B])
		*if2_age = INT_MAX;
#if HZ <= MSEC_PER_SEC
	else if (tdiff > msecs_to_jiffies(INT_MAX))
		*if2_age = INT_MAX;
#endif
	else
		*if2_age = jiffies_to_msecs(tdiff);

	/* Present sequence numbers as if they were incoming on interface */
	*if1_seq = node->seq_out[HSR_PT_SLAVE_B];
	*if2_seq = node->seq_out[HSR_PT_SLAVE_A];

	if (node->AddrB_port != HSR_PT_NONE) {
		port = hsr_port_get_hsr(hsr, node->AddrB_port);
		*addr_b_ifindex = port->dev->ifindex;
	} else {
		*addr_b_ifindex = -1;
	}

	rcu_read_unlock();

	return 0;
}
