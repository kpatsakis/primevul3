static void _free_file_bcast_info_t(file_bcast_info_t *f)
{
	xfree(f->fname);
	if (f->fd)
		close(f->fd);
	xfree(f);
}
