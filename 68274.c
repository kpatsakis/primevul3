UNCURL_EXPORT int32_t uncurl_get_status_code(struct uncurl_conn *ucc, int32_t *status_code)
{
	*status_code = 0;
	return http_get_status_code(ucc->hin, status_code);
}
