UNCURL_EXPORT void uncurl_close(struct uncurl_conn *ucc)
{
	if (!ucc) return;

	tls_close(ucc->tls);
	net_close(ucc->net);
	free(ucc->host);
	http_free_header(ucc->hin);
	free(ucc->hout);
	free(ucc->netbuf);
	free(ucc);
}
