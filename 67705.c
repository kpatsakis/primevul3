int git_index_add(git_index *index, const git_index_entry *source_entry)
{
	git_index_entry *entry = NULL;
	int ret;

	assert(index && source_entry && source_entry->path);

	if (!valid_filemode(source_entry->mode)) {
		giterr_set(GITERR_INDEX, "invalid entry mode");
		return -1;
	}

	if ((ret = index_entry_dup(&entry, index, source_entry)) < 0 ||
		(ret = index_insert(index, &entry, 1, true, true, false)) < 0)
		return ret;

	git_tree_cache_invalidate_path(index->tree, entry->path);
	return 0;
}
