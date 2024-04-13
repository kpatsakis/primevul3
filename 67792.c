static int index_remove_entry(git_index *index, size_t pos)
{
	int error = 0;
	git_index_entry *entry = git_vector_get(&index->entries, pos);

	if (entry != NULL) {
		git_tree_cache_invalidate_path(index->tree, entry->path);
		DELETE_IN_MAP(index, entry);
	}

	error = git_vector_remove(&index->entries, pos);

	if (!error) {
		if (git_atomic_get(&index->readers) > 0) {
			error = git_vector_insert(&index->deleted, entry);
		} else {
			index_entry_free(entry);
		}
	}

	return error;
}
