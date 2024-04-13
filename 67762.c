int git_indexwriter_init_for_operation(
	git_indexwriter *writer,
	git_repository *repo,
	unsigned int *checkout_strategy)
{
	git_index *index;
	int error;

	if ((error = git_repository_index__weakptr(&index, repo)) < 0 ||
		(error = git_indexwriter_init(writer, index)) < 0)
		return error;

	writer->should_write = (*checkout_strategy & GIT_CHECKOUT_DONT_WRITE_INDEX) == 0;
	*checkout_strategy |= GIT_CHECKOUT_DONT_WRITE_INDEX;

	return 0;
}
