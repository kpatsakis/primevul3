void git_index_snapshot_release(git_vector *snap, git_index *index)
{
	git_vector_free(snap);

	git_atomic_dec(&index->readers);

	git_index_free(index);
}
