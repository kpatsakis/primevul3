static int _bcast_find_in_list_to_remove(void *x, void *y)
{
	file_bcast_info_t *f = (file_bcast_info_t *)x;
	time_t *now = (time_t *) y;

	if (f->last_update + FILE_BCAST_TIMEOUT < *now) {
		error("Removing stalled file_bcast transfer from uid "
		      "%u to file `%s`", f->uid, f->fname);
		return true;
	}

	return false;
}
