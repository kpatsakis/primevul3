static void _file_bcast_cleanup(void)
{
	time_t now = time(NULL);

	_fb_wrlock();
	list_delete_all(file_bcast_list, _bcast_find_in_list_to_remove, &now);
	_fb_wrunlock();
}
