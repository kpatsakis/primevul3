static void skb_set_err_queue(struct sk_buff *skb)
{
	/* pkt_type of skbs received on local sockets is never PACKET_OUTGOING.
	 * So, it is safe to (mis)use it to mark skbs on the error queue.
	 */
	skb->pkt_type = PACKET_OUTGOING;
	BUILD_BUG_ON(PACKET_OUTGOING == 0);
}
