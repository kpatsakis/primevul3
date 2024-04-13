static void nfc_llcp_set_nrns(struct nfc_llcp_sock *sock, struct sk_buff *pdu)
{
	pdu->data[2] = (sock->send_n << 4) | (sock->recv_n);
	sock->send_n = (sock->send_n + 1) % 16;
	sock->recv_ack_n = (sock->recv_n - 1) % 16;
}
