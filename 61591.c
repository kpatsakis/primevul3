static void _file_bcast_close_file(file_bcast_info_t *key)
{
	_fb_wrlock();
	list_delete_all(file_bcast_list, _bcast_find_in_list, key);
	_fb_wrunlock();
}
