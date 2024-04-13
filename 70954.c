static void nfc_llcp_tx_work(struct work_struct *work)
{
	struct nfc_llcp_local *local = container_of(work, struct nfc_llcp_local,
						    tx_work);
	struct sk_buff *skb;
	struct sock *sk;
	struct nfc_llcp_sock *llcp_sock;

	skb = skb_dequeue(&local->tx_queue);
	if (skb != NULL) {
		sk = skb->sk;
		llcp_sock = nfc_llcp_sock(sk);

		if (llcp_sock == NULL && nfc_llcp_ptype(skb) == LLCP_PDU_I) {
			kfree_skb(skb);
			nfc_llcp_send_symm(local->dev);
		} else if (llcp_sock && !llcp_sock->remote_ready) {
			skb_queue_head(&local->tx_queue, skb);
			nfc_llcp_send_symm(local->dev);
		} else {
			struct sk_buff *copy_skb = NULL;
			u8 ptype = nfc_llcp_ptype(skb);
			int ret;

			pr_debug("Sending pending skb\n");
			print_hex_dump_debug("LLCP Tx: ", DUMP_PREFIX_OFFSET,
					     16, 1, skb->data, skb->len, true);

			if (ptype == LLCP_PDU_DISC && sk != NULL &&
			    sk->sk_state == LLCP_DISCONNECTING) {
				nfc_llcp_sock_unlink(&local->sockets, sk);
				sock_orphan(sk);
				sock_put(sk);
			}

			if (ptype == LLCP_PDU_I)
				copy_skb = skb_copy(skb, GFP_ATOMIC);

			__net_timestamp(skb);

			nfc_llcp_send_to_raw_sock(local, skb,
						  NFC_DIRECTION_TX);

			ret = nfc_data_exchange(local->dev, local->target_idx,
						skb, nfc_llcp_recv, local);

			if (ret) {
				kfree_skb(copy_skb);
				goto out;
			}

			if (ptype == LLCP_PDU_I && copy_skb)
				skb_queue_tail(&llcp_sock->tx_pending_queue,
					       copy_skb);
		}
	} else {
		nfc_llcp_send_symm(local->dev);
	}

out:
	mod_timer(&local->link_timer,
		  jiffies + msecs_to_jiffies(2 * local->remote_lto));
}
