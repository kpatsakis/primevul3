struct nfc_llcp_sock *nfc_llcp_sock_from_sn(struct nfc_llcp_local *local,
					    u8 *sn, size_t sn_len)
{
	struct sock *sk;
	struct nfc_llcp_sock *llcp_sock, *tmp_sock;

	pr_debug("sn %zd %p\n", sn_len, sn);

	if (sn == NULL || sn_len == 0)
		return NULL;

	read_lock(&local->sockets.lock);

	llcp_sock = NULL;

	sk_for_each(sk, &local->sockets.head) {
		tmp_sock = nfc_llcp_sock(sk);

		pr_debug("llcp sock %p\n", tmp_sock);

		if (tmp_sock->sk.sk_type == SOCK_STREAM &&
		    tmp_sock->sk.sk_state != LLCP_LISTEN)
			continue;

		if (tmp_sock->sk.sk_type == SOCK_DGRAM &&
		    tmp_sock->sk.sk_state != LLCP_BOUND)
			continue;

		if (tmp_sock->service_name == NULL ||
		    tmp_sock->service_name_len == 0)
			continue;

		if (tmp_sock->service_name_len != sn_len)
			continue;

		if (memcmp(sn, tmp_sock->service_name, sn_len) == 0) {
			llcp_sock = tmp_sock;
			break;
		}
	}

	read_unlock(&local->sockets.lock);

	pr_debug("Found llcp sock %p\n", llcp_sock);

	return llcp_sock;
}
