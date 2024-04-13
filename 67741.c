int git_index_remove_bypath(git_index *index, const char *path)
{
	int ret;

	assert(index && path);

	if (((ret = git_index_remove(index, path, 0)) < 0 &&
		ret != GIT_ENOTFOUND) ||
		((ret = index_conflict_to_reuc(index, path)) < 0 &&
		ret != GIT_ENOTFOUND))
		return ret;

	if (ret == GIT_ENOTFOUND)
		giterr_clear();

	return 0;
}
