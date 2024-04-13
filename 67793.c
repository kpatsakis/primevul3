static int index_reuc_insert(
	git_index *index,
	git_index_reuc_entry *reuc)
{
	int res;

	assert(index && reuc && reuc->path != NULL);
	assert(git_vector_is_sorted(&index->reuc));

	res = git_vector_insert_sorted(&index->reuc, reuc, &index_reuc_on_dup);
	return res == GIT_EEXISTS ? 0 : res;
}
