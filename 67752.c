int git_index_snapshot_new(git_vector *snap, git_index *index)
{
	int error;

	GIT_REFCOUNT_INC(index);

	git_atomic_inc(&index->readers);
	git_vector_sort(&index->entries);

	error = git_vector_dup(snap, &index->entries, index->entries._cmp);

	if (error < 0)
		git_index_free(index);

	return error;
}
