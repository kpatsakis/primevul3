void git_index_reuc_clear(git_index *index)
{
	size_t i;

	assert(index);

	for (i = 0; i < index->reuc.length; ++i)
		index_entry_reuc_free(git__swap(index->reuc.contents[i], NULL));

	git_vector_clear(&index->reuc);
}
