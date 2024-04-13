static bool skb_is_err_queue(const struct sk_buff *skb)
{
	/* pkt_type of skbs enqueued on the error queue are set to
	 * PACKET_OUTGOING in skb_set_err_queue(). This is only safe to do
	 * in recvmsg, since skbs received on a local socket will never
	 * have a pkt_type of PACKET_OUTGOING.
	 */
	return skb->pkt_type == PACKET_OUTGOING;
}
