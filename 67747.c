const git_index_reuc_entry *git_index_reuc_get_bypath(
	git_index *index, const char *path)
{
	size_t pos;
	assert(index && path);

	if (!index->reuc.length)
		return NULL;

	assert(git_vector_is_sorted(&index->reuc));

	if (git_index_reuc_find(&pos, index, path) < 0)
		return NULL;

	return git_vector_get(&index->reuc, pos);
}
