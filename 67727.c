int git_index_find_prefix(size_t *at_pos, git_index *index, const char *prefix)
{
	int error = 0;
	size_t pos;
	const git_index_entry *entry;

	index_find(&pos, index, prefix, strlen(prefix), GIT_INDEX_STAGE_ANY);
	entry = git_vector_get(&index->entries, pos);
	if (!entry || git__prefixcmp(entry->path, prefix) != 0)
		error = GIT_ENOTFOUND;

	if (!error && at_pos)
		*at_pos = pos;

	return error;
}
