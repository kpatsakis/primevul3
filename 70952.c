void nfc_llcp_socket_remote_param_init(struct nfc_llcp_sock *sock)
{
	sock->remote_rw = LLCP_DEFAULT_RW;
	sock->remote_miu = LLCP_MAX_MIU + 1;
}
