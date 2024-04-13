static void index_free(git_index *index)
{
	/* index iterators increment the refcount of the index, so if we
	 * get here then there should be no outstanding iterators.
	 */
	assert(!git_atomic_get(&index->readers));

	git_index_clear(index);
	git_idxmap_free(index->entries_map);
	git_vector_free(&index->entries);
	git_vector_free(&index->names);
	git_vector_free(&index->reuc);
	git_vector_free(&index->deleted);

	git__free(index->index_file_path);

	git__memzero(index, sizeof(*index));
	git__free(index);
}
