static void hns_nic_rx_checksum(struct hns_nic_ring_data *ring_data,
				struct sk_buff *skb, u32 flag)
{
	struct net_device *netdev = ring_data->napi.dev;
	u32 l3id;
	u32 l4id;

	/* check if RX checksum offload is enabled */
	if (unlikely(!(netdev->features & NETIF_F_RXCSUM)))
		return;

	/* In hardware, we only support checksum for the following protocols:
	 * 1) IPv4,
	 * 2) TCP(over IPv4 or IPv6),
	 * 3) UDP(over IPv4 or IPv6),
	 * 4) SCTP(over IPv4 or IPv6)
	 * but we support many L3(IPv4, IPv6, MPLS, PPPoE etc) and L4(TCP,
	 * UDP, GRE, SCTP, IGMP, ICMP etc.) protocols.
	 *
	 * Hardware limitation:
	 * Our present hardware RX Descriptor lacks L3/L4 checksum "Status &
	 * Error" bit (which usually can be used to indicate whether checksum
	 * was calculated by the hardware and if there was any error encountered
	 * during checksum calculation).
	 *
	 * Software workaround:
	 * We do get info within the RX descriptor about the kind of L3/L4
	 * protocol coming in the packet and the error status. These errors
	 * might not just be checksum errors but could be related to version,
	 * length of IPv4, UDP, TCP etc.
	 * Because there is no-way of knowing if it is a L3/L4 error due to bad
	 * checksum or any other L3/L4 error, we will not (cannot) convey
	 * checksum status for such cases to upper stack and will not maintain
	 * the RX L3/L4 checksum counters as well.
	 */

	l3id = hnae_get_field(flag, HNS_RXD_L3ID_M, HNS_RXD_L3ID_S);
	l4id = hnae_get_field(flag, HNS_RXD_L4ID_M, HNS_RXD_L4ID_S);

	/*  check L3 protocol for which checksum is supported */
	if ((l3id != HNS_RX_FLAG_L3ID_IPV4) && (l3id != HNS_RX_FLAG_L3ID_IPV6))
		return;

	/* check for any(not just checksum)flagged L3 protocol errors */
	if (unlikely(hnae_get_bit(flag, HNS_RXD_L3E_B)))
		return;

	/* we do not support checksum of fragmented packets */
	if (unlikely(hnae_get_bit(flag, HNS_RXD_FRAG_B)))
		return;

	/*  check L4 protocol for which checksum is supported */
	if ((l4id != HNS_RX_FLAG_L4ID_TCP) &&
	    (l4id != HNS_RX_FLAG_L4ID_UDP) &&
	    (l4id != HNS_RX_FLAG_L4ID_SCTP))
		return;

	/* check for any(not just checksum)flagged L4 protocol errors */
	if (unlikely(hnae_get_bit(flag, HNS_RXD_L4E_B)))
		return;

	/* now, this has to be a packet with valid RX checksum */
	skb->ip_summed = CHECKSUM_UNNECESSARY;
}
