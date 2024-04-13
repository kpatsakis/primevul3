UNCURL_EXPORT void uncurl_free_header(struct uncurl_conn *ucc)
{
	free(ucc->hout);
	ucc->hout = NULL;
}
