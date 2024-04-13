static struct nfc_llcp_sock *nfc_llcp_sock_get_sn(struct nfc_llcp_local *local,
						  u8 *sn, size_t sn_len)
{
	struct nfc_llcp_sock *llcp_sock;

	llcp_sock = nfc_llcp_sock_from_sn(local, sn, sn_len);

	if (llcp_sock == NULL)
		return NULL;

	sock_hold(&llcp_sock->sk);

	return llcp_sock;
}
