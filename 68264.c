UNCURL_EXPORT int32_t uncurl_accept(struct uncurl_tls_ctx *uc_tls, struct uncurl_conn *ucc,
	struct uncurl_conn **ucc_new_in, int32_t scheme)
{
	int32_t r = UNCURL_ERR_DEFAULT;
	int32_t e;

	struct uncurl_conn *ucc_new = *ucc_new_in = uncurl_new_conn(ucc);

	struct net_context *new_net = NULL;

	e = net_accept(ucc->net, &new_net);
	if (e != UNCURL_OK) {r = e; goto uncurl_accept_end;}

	ucc_new->net = new_net;
	uncurl_attach_net(ucc_new);

	if (scheme == UNCURL_HTTPS || scheme == UNCURL_WSS) {
		if (!uc_tls) return UNCURL_TLS_ERR_CONTEXT;

		e = tls_accept(&ucc_new->tls, uc_tls->tlss, ucc_new->net, &ucc_new->topts);
		if (e != UNCURL_OK) {r = e; goto uncurl_accept_end;}

		uncurl_attach_tls(ucc_new);
	}

	return UNCURL_OK;

	uncurl_accept_end:

	free(ucc_new);
	*ucc_new_in = NULL;

	return r;
}
