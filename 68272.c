UNCURL_EXPORT int32_t uncurl_get_header(struct uncurl_conn *ucc, char *key, int32_t *val_int, char **val_str)
{
	return http_get_header(ucc->hin, key, val_int, val_str);
}
