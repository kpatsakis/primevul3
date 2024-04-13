_warc_cleanup(struct archive_read *a)
{
	struct warc_s *w = a->format->data;

	if (w->pool.len > 0U) {
		free(w->pool.str);
	}
	archive_string_free(&w->sver);
	free(w);
	a->format->data = NULL;
	return (ARCHIVE_OK);
}
