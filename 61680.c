void file_bcast_purge(void)
{
	_fb_wrlock();
	list_destroy(file_bcast_list);
	/* destroying list before exit, no need to unlock */
}
