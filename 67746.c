const git_index_reuc_entry *git_index_reuc_get_byindex(
	git_index *index, size_t n)
{
	assert(index);
	assert(git_vector_is_sorted(&index->reuc));

	return git_vector_get(&index->reuc, n);
}
