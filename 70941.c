void nfc_llcp_send_to_raw_sock(struct nfc_llcp_local *local,
			       struct sk_buff *skb, u8 direction)
{
	struct sk_buff *skb_copy = NULL, *nskb;
	struct sock *sk;
	u8 *data;

	read_lock(&local->raw_sockets.lock);

	sk_for_each(sk, &local->raw_sockets.head) {
		if (sk->sk_state != LLCP_BOUND)
			continue;

		if (skb_copy == NULL) {
			skb_copy = __pskb_copy_fclone(skb, NFC_RAW_HEADER_SIZE,
						      GFP_ATOMIC, true);

			if (skb_copy == NULL)
				continue;

			data = skb_push(skb_copy, NFC_RAW_HEADER_SIZE);

			data[0] = local->dev ? local->dev->idx : 0xFF;
			data[1] = direction & 0x01;
			data[1] |= (RAW_PAYLOAD_LLCP << 1);
		}

		nskb = skb_clone(skb_copy, GFP_ATOMIC);
		if (!nskb)
			continue;

		if (sock_queue_rcv_skb(sk, nskb))
			kfree_skb(nskb);
	}

	read_unlock(&local->raw_sockets.lock);

	kfree_skb(skb_copy);
}
