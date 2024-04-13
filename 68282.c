static int32_t uncurl_response_body_chunked(struct uncurl_conn *ucc, char **body, uint32_t *body_len)
{
	uint32_t offset = 0;
	uint32_t chunk_len = 0;

	do {
		int32_t e;

		e = uncurl_read_chunk_len(ucc, &chunk_len);
		if (e != UNCURL_OK) return e;
		if (offset + chunk_len > ucc->opts.max_body) return UNCURL_ERR_MAX_BODY;

		*body = realloc(*body, offset + chunk_len + 2);

		e = ucc->read(ucc->ctx, *body + offset, chunk_len + 2);
		if (e != UNCURL_OK) return e;

		offset += chunk_len;

	} while (chunk_len > 0);

	(*body)[offset] = '\0';
	*body_len = offset;

	return UNCURL_OK;
}
