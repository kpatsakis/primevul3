static void nfc_llcp_recv_hdlc(struct nfc_llcp_local *local,
			       struct sk_buff *skb)
{
	struct nfc_llcp_sock *llcp_sock;
	struct sock *sk;
	u8 dsap, ssap, ptype, ns, nr;

	ptype = nfc_llcp_ptype(skb);
	dsap = nfc_llcp_dsap(skb);
	ssap = nfc_llcp_ssap(skb);
	ns = nfc_llcp_ns(skb);
	nr = nfc_llcp_nr(skb);

	pr_debug("%d %d R %d S %d\n", dsap, ssap, nr, ns);

	llcp_sock = nfc_llcp_sock_get(local, dsap, ssap);
	if (llcp_sock == NULL) {
		nfc_llcp_send_dm(local, dsap, ssap, LLCP_DM_NOCONN);
		return;
	}

	sk = &llcp_sock->sk;
	lock_sock(sk);
	if (sk->sk_state == LLCP_CLOSED) {
		release_sock(sk);
		nfc_llcp_sock_put(llcp_sock);
	}

	/* Pass the payload upstream */
	if (ptype == LLCP_PDU_I) {
		pr_debug("I frame, queueing on %p\n", &llcp_sock->sk);

		if (ns == llcp_sock->recv_n)
			llcp_sock->recv_n = (llcp_sock->recv_n + 1) % 16;
		else
			pr_err("Received out of sequence I PDU\n");

		skb_pull(skb, LLCP_HEADER_SIZE + LLCP_SEQUENCE_SIZE);
		if (!sock_queue_rcv_skb(&llcp_sock->sk, skb)) {
			/*
			 * I frames will be freed from the socket layer, so we
			 * need to keep them alive until someone receives them.
			 */
			skb_get(skb);
		} else {
			pr_err("Receive queue is full\n");
		}
	}

	/* Remove skbs from the pending queue */
	if (llcp_sock->send_ack_n != nr) {
		struct sk_buff *s, *tmp;
		u8 n;

		llcp_sock->send_ack_n = nr;

		/* Remove and free all skbs until ns == nr */
		skb_queue_walk_safe(&llcp_sock->tx_pending_queue, s, tmp) {
			n = nfc_llcp_ns(s);

			skb_unlink(s, &llcp_sock->tx_pending_queue);
			kfree_skb(s);

			if (n == nr)
				break;
		}

		/* Re-queue the remaining skbs for transmission */
		skb_queue_reverse_walk_safe(&llcp_sock->tx_pending_queue,
					    s, tmp) {
			skb_unlink(s, &llcp_sock->tx_pending_queue);
			skb_queue_head(&local->tx_queue, s);
		}
	}

	if (ptype == LLCP_PDU_RR)
		llcp_sock->remote_ready = true;
	else if (ptype == LLCP_PDU_RNR)
		llcp_sock->remote_ready = false;

	if (nfc_llcp_queue_i_frames(llcp_sock) == 0 && ptype == LLCP_PDU_I)
		nfc_llcp_send_rr(llcp_sock);

	release_sock(sk);
	nfc_llcp_sock_put(llcp_sock);
}
