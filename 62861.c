_warc_skip(struct archive_read *a)
{
	struct warc_s *w = a->format->data;

	__archive_read_consume(a, w->cntlen + 4U/*\r\n\r\n separator*/);
	w->cntlen = 0U;
	w->cntoff = 0U;
	return (ARCHIVE_OK);
}
