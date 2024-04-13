UNCURL_EXPORT void uncurl_free_info(struct uncurl_info *uci)
{
	free(uci->host);
	free(uci->path);
}
