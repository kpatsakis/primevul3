UNCURL_EXPORT void uncurl_set_header_int(struct uncurl_conn *ucc, char *name, int32_t value)
{
	ucc->hout = http_set_header(ucc->hout, name, HTTP_INT, &value);
}
