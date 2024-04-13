UNCURL_EXPORT int32_t uncurl_ws_write(struct uncurl_conn *ucc, char *buf, uint32_t buf_len, uint8_t opcode)
{
	struct ws_header h;
	memset(&h, 0, sizeof(struct ws_header));

	h.fin = 1;
	h.mask = ucc->ws_mask;
	h.opcode = opcode;
	h.payload_len = buf_len;

	if (h.payload_len + WS_HEADER_SIZE > ucc->netbuf_size) {
		free(ucc->netbuf);
		ucc->netbuf_size = h.payload_len + WS_HEADER_SIZE;
		ucc->netbuf = malloc((size_t) ucc->netbuf_size);
	}

	uint64_t out_size = 0;
	ws_serialize(&h, &ucc->seed, buf, ucc->netbuf, &out_size);

	return ucc->write(ucc->ctx, ucc->netbuf, (uint32_t) out_size);
}
