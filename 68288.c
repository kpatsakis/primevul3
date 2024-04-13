UNCURL_EXPORT int32_t uncurl_write_body(struct uncurl_conn *ucc, char *body, uint32_t body_len)
{
	int32_t e;

	e = ucc->write(ucc->ctx, body, body_len);

	return e;
}
