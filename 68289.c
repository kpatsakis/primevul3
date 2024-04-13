UNCURL_EXPORT int32_t uncurl_write_header(struct uncurl_conn *ucc, char *str0, char *str1, int32_t type)
{
	int32_t e;

	char *h = (type == UNCURL_REQUEST) ?
		http_request(str0, ucc->host, str1, ucc->hout) :
		http_response(str0, str1, ucc->hout);

	e = ucc->write(ucc->ctx, h, (uint32_t) strlen(h));

	free(h);

	return e;
}
