UNCURL_EXPORT void uncurl_get_socket(struct uncurl_conn *ucc, void *socket)
{
	net_get_socket(ucc->net, socket);
}
