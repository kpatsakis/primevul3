static void nfc_llcp_socket_purge(struct nfc_llcp_sock *sock)
{
	struct nfc_llcp_local *local = sock->local;
	struct sk_buff *s, *tmp;

	pr_debug("%p\n", &sock->sk);

	skb_queue_purge(&sock->tx_queue);
	skb_queue_purge(&sock->tx_pending_queue);

	if (local == NULL)
		return;

	/* Search for local pending SKBs that are related to this socket */
	skb_queue_walk_safe(&local->tx_queue, s, tmp) {
		if (s->sk != &sock->sk)
			continue;

		skb_unlink(s, &local->tx_queue);
		kfree_skb(s);
	}
}
