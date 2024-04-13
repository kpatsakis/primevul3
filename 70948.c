static void nfc_llcp_sock_put(struct nfc_llcp_sock *sock)
{
	sock_put(&sock->sk);
}
