int git_index_snapshot_find(
	size_t *out, git_vector *entries, git_vector_cmp entry_srch,
	const char *path, size_t path_len, int stage)
{
	return index_find_in_entries(out, entries, entry_srch, path, path_len, stage);
}
