UNCURL_EXPORT int32_t uncurl_ws_close(struct uncurl_conn *ucc, uint16_t status_code)
{
	uint16_t status_code_be = htons(status_code);

	return uncurl_ws_write(ucc, (char *) &status_code_be, sizeof(uint16_t), UNCURL_WSOP_CLOSE);
}
