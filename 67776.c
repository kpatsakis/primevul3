static int index_entry_init(
	git_index_entry **entry_out,
	git_index *index,
	const char *rel_path)
{
	int error = 0;
	git_index_entry *entry = NULL;
	struct stat st;
	git_oid oid;

	if (INDEX_OWNER(index) == NULL)
		return create_index_error(-1,
			"could not initialize index entry. "
			"Index is not backed up by an existing repository.");

	if (index_entry_create(&entry, INDEX_OWNER(index), rel_path, true) < 0)
		return -1;

	/* write the blob to disk and get the oid and stat info */
	error = git_blob__create_from_paths(
		&oid, &st, INDEX_OWNER(index), NULL, rel_path, 0, true);

	if (error < 0) {
		index_entry_free(entry);
		return error;
	}

	entry->id = oid;
	git_index_entry__init_from_stat(entry, &st, !index->distrust_filemode);

	*entry_out = (git_index_entry *)entry;
	return 0;
}
