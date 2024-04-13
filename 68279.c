UNCURL_EXPORT int32_t uncurl_read_body_all(struct uncurl_conn *ucc, char **body, uint32_t *body_len)
{
	int32_t r = UNCURL_ERR_DEFAULT;
	int32_t e;

	*body = NULL;
	*body_len = 0;

	if (uncurl_check_header(ucc, "Transfer-Encoding", "chunked")) {
		e = uncurl_response_body_chunked(ucc, body, body_len);
		if (e != UNCURL_OK) {r = e; goto uncurl_response_body_end;}

		r = UNCURL_OK;
	}

	if (r != UNCURL_OK) {
		e = uncurl_get_header_int(ucc, "Content-Length", (int32_t *) body_len);
		if (e != UNCURL_OK) {r = e; goto uncurl_response_body_end;}

		if (*body_len == 0) {r = UNCURL_ERR_NO_BODY; goto uncurl_response_body_end;}
		if (*body_len > ucc->opts.max_body) {r = UNCURL_ERR_MAX_BODY; goto uncurl_response_body_end;}

		*body = calloc(*body_len + 1, 1);

		e = ucc->read(ucc->ctx, *body, *body_len);

		if (e != UNCURL_OK) {r = e; goto uncurl_response_body_end;}

		r = UNCURL_OK;
	}

	uncurl_response_body_end:

	if (r != UNCURL_OK) {
		free(*body);
		*body = NULL;
	}

	return r;
}
