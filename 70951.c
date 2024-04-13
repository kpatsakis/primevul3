static void nfc_llcp_socket_release(struct nfc_llcp_local *local, bool device,
				    int err)
{
	struct sock *sk;
	struct hlist_node *tmp;
	struct nfc_llcp_sock *llcp_sock;

	skb_queue_purge(&local->tx_queue);

	write_lock(&local->sockets.lock);

	sk_for_each_safe(sk, tmp, &local->sockets.head) {
		llcp_sock = nfc_llcp_sock(sk);

		bh_lock_sock(sk);

		nfc_llcp_socket_purge(llcp_sock);

		if (sk->sk_state == LLCP_CONNECTED)
			nfc_put_device(llcp_sock->dev);

		if (sk->sk_state == LLCP_LISTEN) {
			struct nfc_llcp_sock *lsk, *n;
			struct sock *accept_sk;

			list_for_each_entry_safe(lsk, n,
						 &llcp_sock->accept_queue,
						 accept_queue) {
				accept_sk = &lsk->sk;
				bh_lock_sock(accept_sk);

				nfc_llcp_accept_unlink(accept_sk);

				if (err)
					accept_sk->sk_err = err;
				accept_sk->sk_state = LLCP_CLOSED;
				accept_sk->sk_state_change(sk);

				bh_unlock_sock(accept_sk);
			}
		}

		if (err)
			sk->sk_err = err;
		sk->sk_state = LLCP_CLOSED;
		sk->sk_state_change(sk);

		bh_unlock_sock(sk);

		sk_del_node_init(sk);
	}

	write_unlock(&local->sockets.lock);

	/* If we still have a device, we keep the RAW sockets alive */
	if (device == true)
		return;

	write_lock(&local->raw_sockets.lock);

	sk_for_each_safe(sk, tmp, &local->raw_sockets.head) {
		llcp_sock = nfc_llcp_sock(sk);

		bh_lock_sock(sk);

		nfc_llcp_socket_purge(llcp_sock);

		if (err)
			sk->sk_err = err;
		sk->sk_state = LLCP_CLOSED;
		sk->sk_state_change(sk);

		bh_unlock_sock(sk);

		sk_del_node_init(sk);
	}

	write_unlock(&local->raw_sockets.lock);
}
