int nfc_llcp_queue_i_frames(struct nfc_llcp_sock *sock)
{
	int nr_frames = 0;
	struct nfc_llcp_local *local = sock->local;

	pr_debug("Remote ready %d tx queue len %d remote rw %d",
		 sock->remote_ready, skb_queue_len(&sock->tx_pending_queue),
		 sock->remote_rw);

	/* Try to queue some I frames for transmission */
	while (sock->remote_ready &&
	       skb_queue_len(&sock->tx_pending_queue) < sock->remote_rw) {
		struct sk_buff *pdu;

		pdu = skb_dequeue(&sock->tx_queue);
		if (pdu == NULL)
			break;

		/* Update N(S)/N(R) */
		nfc_llcp_set_nrns(sock, pdu);

		skb_queue_tail(&local->tx_queue, pdu);
		nr_frames++;
	}

	return nr_frames;
}
