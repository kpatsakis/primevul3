void git_index_free(git_index *index)
{
	if (index == NULL)
		return;

	GIT_REFCOUNT_DEC(index, index_free);
}
