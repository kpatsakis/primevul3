UNCURL_EXPORT int8_t uncurl_check_header(struct uncurl_conn *ucc, char *name, char *subval)
{
	int32_t e;
	char *val = NULL;

	e = uncurl_get_header_str(ucc, name, &val);
	if (e == UNCURL_OK && strstr(http_lc(val), subval)) return 1;

	return 0;
}
