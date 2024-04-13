UNCURL_EXPORT int32_t uncurl_listen(struct uncurl_conn *ucc, char *bind_ip4, uint16_t port)
{
	int32_t e;

	ucc->port = port;

	e = net_listen(&ucc->net, bind_ip4, ucc->port, &ucc->nopts);
	if (e != UNCURL_OK) return e;

	return UNCURL_OK;
}
