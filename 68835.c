static void fill_v2_desc(struct hnae_ring *ring, void *priv,
			 int size, dma_addr_t dma, int frag_end,
			 int buf_num, enum hns_desc_type type, int mtu)
{
	struct hnae_desc *desc = &ring->desc[ring->next_to_use];
	struct hnae_desc_cb *desc_cb = &ring->desc_cb[ring->next_to_use];
	struct iphdr *iphdr;
	struct ipv6hdr *ipv6hdr;
	struct sk_buff *skb;
	__be16 protocol;
	u8 bn_pid = 0;
	u8 rrcfv = 0;
	u8 ip_offset = 0;
	u8 tvsvsn = 0;
	u16 mss = 0;
	u8 l4_len = 0;
	u16 paylen = 0;

	desc_cb->priv = priv;
	desc_cb->length = size;
	desc_cb->dma = dma;
	desc_cb->type = type;

	desc->addr = cpu_to_le64(dma);
	desc->tx.send_size = cpu_to_le16((u16)size);

	/* config bd buffer end */
	hnae_set_bit(rrcfv, HNSV2_TXD_VLD_B, 1);
	hnae_set_field(bn_pid, HNSV2_TXD_BUFNUM_M, 0, buf_num - 1);

	/* fill port_id in the tx bd for sending management pkts */
	hnae_set_field(bn_pid, HNSV2_TXD_PORTID_M,
		       HNSV2_TXD_PORTID_S, ring->q->handle->dport_id);

	if (type == DESC_TYPE_SKB) {
		skb = (struct sk_buff *)priv;

		if (skb->ip_summed == CHECKSUM_PARTIAL) {
			skb_reset_mac_len(skb);
			protocol = skb->protocol;
			ip_offset = ETH_HLEN;

			if (protocol == htons(ETH_P_8021Q)) {
				ip_offset += VLAN_HLEN;
				protocol = vlan_get_protocol(skb);
				skb->protocol = protocol;
			}

			if (skb->protocol == htons(ETH_P_IP)) {
				iphdr = ip_hdr(skb);
				hnae_set_bit(rrcfv, HNSV2_TXD_L3CS_B, 1);
				hnae_set_bit(rrcfv, HNSV2_TXD_L4CS_B, 1);

				/* check for tcp/udp header */
				if (iphdr->protocol == IPPROTO_TCP &&
				    skb_is_gso(skb)) {
					hnae_set_bit(tvsvsn,
						     HNSV2_TXD_TSE_B, 1);
					l4_len = tcp_hdrlen(skb);
					mss = skb_shinfo(skb)->gso_size;
					paylen = skb->len - SKB_TMP_LEN(skb);
				}
			} else if (skb->protocol == htons(ETH_P_IPV6)) {
				hnae_set_bit(tvsvsn, HNSV2_TXD_IPV6_B, 1);
				ipv6hdr = ipv6_hdr(skb);
				hnae_set_bit(rrcfv, HNSV2_TXD_L4CS_B, 1);

				/* check for tcp/udp header */
				if (ipv6hdr->nexthdr == IPPROTO_TCP &&
				    skb_is_gso(skb) && skb_is_gso_v6(skb)) {
					hnae_set_bit(tvsvsn,
						     HNSV2_TXD_TSE_B, 1);
					l4_len = tcp_hdrlen(skb);
					mss = skb_shinfo(skb)->gso_size;
					paylen = skb->len - SKB_TMP_LEN(skb);
				}
			}
			desc->tx.ip_offset = ip_offset;
			desc->tx.tse_vlan_snap_v6_sctp_nth = tvsvsn;
			desc->tx.mss = cpu_to_le16(mss);
			desc->tx.l4_len = l4_len;
			desc->tx.paylen = cpu_to_le16(paylen);
		}
	}

	hnae_set_bit(rrcfv, HNSV2_TXD_FE_B, frag_end);

	desc->tx.bn_pid = bn_pid;
	desc->tx.ra_ri_cs_fe_vld = rrcfv;

	ring_ptr_move_fw(ring, next_to_use);
}
