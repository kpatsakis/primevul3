void git_index_conflict_iterator_free(git_index_conflict_iterator *iterator)
{
	if (iterator == NULL)
		return;

	git__free(iterator);
}
