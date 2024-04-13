int git_index_conflict_get(
	const git_index_entry **ancestor_out,
	const git_index_entry **our_out,
	const git_index_entry **their_out,
	git_index *index,
	const char *path)
{
	size_t pos;
	int len = 0;

	assert(ancestor_out && our_out && their_out && index && path);

	*ancestor_out = NULL;
	*our_out = NULL;
	*their_out = NULL;

	if (git_index_find(&pos, index, path) < 0)
		return GIT_ENOTFOUND;

	if ((len = index_conflict__get_byindex(
		ancestor_out, our_out, their_out, index, pos)) < 0)
		return len;
	else if (len == 0)
		return GIT_ENOTFOUND;

	return 0;
}
