int git_index_read_index(
	git_index *index,
	const git_index *new_index)
{
	git_iterator *new_iterator = NULL;
	git_iterator_options opts = GIT_ITERATOR_OPTIONS_INIT;
	int error;

	opts.flags = GIT_ITERATOR_DONT_IGNORE_CASE |
		GIT_ITERATOR_INCLUDE_CONFLICTS;

	if ((error = git_iterator_for_index(&new_iterator,
		git_index_owner(new_index), (git_index *)new_index, &opts)) < 0 ||
		(error = git_index_read_iterator(index, new_iterator,
		new_index->entries.length)) < 0)
		goto done;

done:
	git_iterator_free(new_iterator);
	return error;
}
