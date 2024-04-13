int git_index_open(git_index **index_out, const char *index_path)
{
	git_index *index;
	int error = -1;

	assert(index_out);

	index = git__calloc(1, sizeof(git_index));
	GITERR_CHECK_ALLOC(index);

	git_pool_init(&index->tree_pool, 1);

	if (index_path != NULL) {
		index->index_file_path = git__strdup(index_path);
		if (!index->index_file_path)
			goto fail;

		/* Check if index file is stored on disk already */
		if (git_path_exists(index->index_file_path) == true)
			index->on_disk = 1;
	}

	if (git_vector_init(&index->entries, 32, git_index_entry_cmp) < 0 ||
		git_idxmap_alloc(&index->entries_map) < 0 ||
		git_vector_init(&index->names, 8, conflict_name_cmp) < 0 ||
		git_vector_init(&index->reuc, 8, reuc_cmp) < 0 ||
		git_vector_init(&index->deleted, 8, git_index_entry_cmp) < 0)
		goto fail;

	index->entries_cmp_path = git__strcmp_cb;
	index->entries_search = git_index_entry_srch;
	index->entries_search_path = index_entry_srch_path;
	index->reuc_search = reuc_srch;
	index->version = INDEX_VERSION_NUMBER_DEFAULT;

	if (index_path != NULL && (error = git_index_read(index, true)) < 0)
		goto fail;

	*index_out = index;
	GIT_REFCOUNT_INC(index);

	return 0;

fail:
	git_pool_clear(&index->tree_pool);
	git_index_free(index);
	return error;
}
