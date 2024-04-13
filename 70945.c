static struct nfc_llcp_sock *nfc_llcp_sock_get(struct nfc_llcp_local *local,
					       u8 ssap, u8 dsap)
{
	struct sock *sk;
	struct nfc_llcp_sock *llcp_sock, *tmp_sock;

	pr_debug("ssap dsap %d %d\n", ssap, dsap);

	if (ssap == 0 && dsap == 0)
		return NULL;

	read_lock(&local->sockets.lock);

	llcp_sock = NULL;

	sk_for_each(sk, &local->sockets.head) {
		tmp_sock = nfc_llcp_sock(sk);

		if (tmp_sock->ssap == ssap && tmp_sock->dsap == dsap) {
			llcp_sock = tmp_sock;
			break;
		}
	}

	read_unlock(&local->sockets.lock);

	if (llcp_sock == NULL)
		return NULL;

	sock_hold(&llcp_sock->sk);

	return llcp_sock;
}
