static int32_t uncurl_read_header_(struct uncurl_conn *ucc, char **header)
{
	int32_t r = UNCURL_ERR_MAX_HEADER;

	uint32_t max_header = ucc->opts.max_header;
	char *h = *header = calloc(max_header, 1);

	uint32_t x = 0;
	for (; x < max_header - 1; x++) {
		int32_t e;

		e = ucc->read(ucc->ctx, h + x, 1);
		if (e != UNCURL_OK) {r = e; break;}

		if (x > 2 && h[x - 3] == '\r' && h[x - 2] == '\n' && h[x - 1] == '\r' && h[x] == '\n')
			return UNCURL_OK;
	}

	free(h);
	*header = NULL;

	return r;
}
