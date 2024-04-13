static file_bcast_info_t *_bcast_lookup_file(file_bcast_info_t *key)
{
	return list_find_first(file_bcast_list, _bcast_find_in_list, key);
}
