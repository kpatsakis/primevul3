int git_index_conflict_iterator_new(
	git_index_conflict_iterator **iterator_out,
	git_index *index)
{
	git_index_conflict_iterator *it = NULL;

	assert(iterator_out && index);

	it = git__calloc(1, sizeof(git_index_conflict_iterator));
	GITERR_CHECK_ALLOC(it);

	it->index = index;

	*iterator_out = it;
	return 0;
}
