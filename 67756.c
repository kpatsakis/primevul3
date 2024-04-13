int git_index_write(git_index *index)
{
	git_indexwriter writer = GIT_INDEXWRITER_INIT;
	int error;

	truncate_racily_clean(index);

	if ((error = git_indexwriter_init(&writer, index)) == 0)
		error = git_indexwriter_commit(&writer);

	git_indexwriter_cleanup(&writer);

	return error;
}
