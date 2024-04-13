static void nfc_llcp_recv_disc(struct nfc_llcp_local *local,
			       struct sk_buff *skb)
{
	struct nfc_llcp_sock *llcp_sock;
	struct sock *sk;
	u8 dsap, ssap;

	dsap = nfc_llcp_dsap(skb);
	ssap = nfc_llcp_ssap(skb);

	if ((dsap == 0) && (ssap == 0)) {
		pr_debug("Connection termination");
		nfc_dep_link_down(local->dev);
		return;
	}

	llcp_sock = nfc_llcp_sock_get(local, dsap, ssap);
	if (llcp_sock == NULL) {
		nfc_llcp_send_dm(local, dsap, ssap, LLCP_DM_NOCONN);
		return;
	}

	sk = &llcp_sock->sk;
	lock_sock(sk);

	nfc_llcp_socket_purge(llcp_sock);

	if (sk->sk_state == LLCP_CLOSED) {
		release_sock(sk);
		nfc_llcp_sock_put(llcp_sock);
	}

	if (sk->sk_state == LLCP_CONNECTED) {
		nfc_put_device(local->dev);
		sk->sk_state = LLCP_CLOSED;
		sk->sk_state_change(sk);
	}

	nfc_llcp_send_dm(local, dsap, ssap, LLCP_DM_DISC);

	release_sock(sk);
	nfc_llcp_sock_put(llcp_sock);
}
