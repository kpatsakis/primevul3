int git_index_reuc_remove(git_index *index, size_t position)
{
	int error;
	git_index_reuc_entry *reuc;

	assert(git_vector_is_sorted(&index->reuc));

	reuc = git_vector_get(&index->reuc, position);
	error = git_vector_remove(&index->reuc, position);

	if (!error)
		index_entry_reuc_free(reuc);

	return error;
}
