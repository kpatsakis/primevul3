int git_index_add_frombuffer(
    git_index *index, const git_index_entry *source_entry,
    const void *buffer, size_t len)
{
	git_index_entry *entry = NULL;
	int error = 0;
	git_oid id;

	assert(index && source_entry->path);

	if (INDEX_OWNER(index) == NULL)
		return create_index_error(-1,
			"could not initialize index entry. "
			"Index is not backed up by an existing repository.");

	if (!is_file_or_link(source_entry->mode)) {
		giterr_set(GITERR_INDEX, "invalid filemode");
		return -1;
	}

	if (index_entry_dup(&entry, index, source_entry) < 0)
		return -1;

	error = git_blob_create_frombuffer(&id, INDEX_OWNER(index), buffer, len);
	if (error < 0) {
		index_entry_free(entry);
		return error;
	}

	git_oid_cpy(&entry->id, &id);
	entry->file_size = len;

	if ((error = index_insert(index, &entry, 1, true, true, true)) < 0)
		return error;

	/* Adding implies conflict was resolved, move conflict entries to REUC */
	if ((error = index_conflict_to_reuc(index, entry->path)) < 0 && error != GIT_ENOTFOUND)
		return error;

	git_tree_cache_invalidate_path(index->tree, entry->path);
	return 0;
}
