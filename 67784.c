GIT_INLINE(int) index_find(
	size_t *out, git_index *index,
	const char *path, size_t path_len, int stage)
{
	git_vector_sort(&index->entries);

	return index_find_in_entries(
		out, &index->entries, index->entries_search, path, path_len, stage);
}
