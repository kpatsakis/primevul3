int git_index_clear(git_index *index)
{
	int error = 0;

	assert(index);

	index->tree = NULL;
	git_pool_clear(&index->tree_pool);

	git_idxmap_clear(index->entries_map);
	while (!error && index->entries.length > 0)
		error = index_remove_entry(index, index->entries.length - 1);
	index_free_deleted(index);

	git_index_reuc_clear(index);
	git_index_name_clear(index);

	git_futils_filestamp_set(&index->stamp, NULL);

	return error;
}
