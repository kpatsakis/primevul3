UNCURL_EXPORT int32_t uncurl_connect(struct uncurl_tls_ctx *uc_tls, struct uncurl_conn *ucc,
	int32_t scheme, char *host, uint16_t port)
{
	int32_t e;

	ucc->host = strdup(host);
	ucc->port = port;

	char ip4[LEN_IP4];
	e = net_getip4(ucc->host, ip4, LEN_IP4);
	if (e != UNCURL_OK) return e;

	e = net_connect(&ucc->net, ip4, ucc->port, &ucc->nopts);
	if (e != UNCURL_OK) return e;

	uncurl_attach_net(ucc);

	if (scheme == UNCURL_HTTPS || scheme == UNCURL_WSS) {
		if (!uc_tls) return UNCURL_TLS_ERR_CONTEXT;

		e = tls_connect(&ucc->tls, uc_tls->tlss, ucc->net, ucc->host, &ucc->topts);
		if (e != UNCURL_OK) return e;

		uncurl_attach_tls(ucc);
	}

	return UNCURL_OK;
}
