static int _bcast_find_in_list(void *x, void *y)
{
	file_bcast_info_t *info = (file_bcast_info_t *)x;
	file_bcast_info_t *key = (file_bcast_info_t *)y;
	/* uid, job_id, and fname must match */
	return ((info->uid == key->uid)
		&& (info->job_id == key->job_id)
		&& (!xstrcmp(info->fname, key->fname)));
}
