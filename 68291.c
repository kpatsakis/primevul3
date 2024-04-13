UNCURL_EXPORT int32_t uncurl_ws_connect(struct uncurl_conn *ucc, char *path, char *origin)
{
	int32_t e;
	int32_t r = UNCURL_ERR_DEFAULT;

	char *sec_key = ws_create_key(&ucc->seed);
	uncurl_set_header_str(ucc, "Upgrade", "websocket");
	uncurl_set_header_str(ucc, "Connection", "Upgrade");
	uncurl_set_header_str(ucc, "Sec-WebSocket-Key", sec_key);
	uncurl_set_header_str(ucc, "Sec-WebSocket-Version", "13");

	if (origin)
		uncurl_set_header_str(ucc, "Origin", origin);

	e = uncurl_write_header(ucc, "GET", path, UNCURL_REQUEST);
	if (e != UNCURL_OK) {r = e; goto uncurl_ws_connect_end;}

	e = uncurl_read_header(ucc);
	if (e != UNCURL_OK) {r = e; goto uncurl_ws_connect_end;}

	int32_t status_code = 0;
	e = uncurl_get_status_code(ucc, &status_code);
	if (e != UNCURL_OK) {r = e; goto uncurl_ws_connect_end;}
	if (status_code != 101) {r = UNCURL_WS_ERR_STATUS; goto uncurl_ws_connect_end;}

	char *server_sec_key = NULL;
	e = uncurl_get_header_str(ucc, "Sec-WebSocket-Accept", &server_sec_key);
	if (e != UNCURL_OK) {r = e; goto uncurl_ws_connect_end;}
	if (!ws_validate_key(sec_key, server_sec_key)) {r = UNCURL_WS_ERR_KEY; goto uncurl_ws_connect_end;}

	ucc->ws_mask = 1;

	r = UNCURL_OK;

	uncurl_ws_connect_end:

	free(sec_key);

	return r;
}
