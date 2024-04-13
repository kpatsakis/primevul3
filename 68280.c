static int32_t uncurl_read_chunk_len(struct uncurl_conn *ucc, uint32_t *len)
{
	int32_t r = UNCURL_ERR_MAX_CHUNK;

	char chunk_len[LEN_CHUNK];
	memset(chunk_len, 0, LEN_CHUNK);

	for (uint32_t x = 0; x < LEN_CHUNK - 1; x++) {
		int32_t e;

		e = ucc->read(ucc->ctx, chunk_len + x, 1);
		if (e != UNCURL_OK) {r = e; break;}

		if (x > 0 && chunk_len[x - 1] == '\r' && chunk_len[x] == '\n') {
			chunk_len[x - 1] = '\0';
			*len = strtoul(chunk_len, NULL, 16);
			return UNCURL_OK;
		}
	}

	*len = 0;

	return r;
}
