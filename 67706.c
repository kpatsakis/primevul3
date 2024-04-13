int git_index_add_all(
	git_index *index,
	const git_strarray *paths,
	unsigned int flags,
	git_index_matched_path_cb cb,
	void *payload)
{
	int error;
	git_repository *repo;
	git_iterator *wditer = NULL;
	git_pathspec ps;
	bool no_fnmatch = (flags & GIT_INDEX_ADD_DISABLE_PATHSPEC_MATCH) != 0;

	assert(index);

	repo = INDEX_OWNER(index);
	if ((error = git_repository__ensure_not_bare(repo, "index add all")) < 0)
		return error;

	if ((error = git_pathspec__init(&ps, paths)) < 0)
		return error;

	/* optionally check that pathspec doesn't mention any ignored files */
	if ((flags & GIT_INDEX_ADD_CHECK_PATHSPEC) != 0 &&
		(flags & GIT_INDEX_ADD_FORCE) == 0 &&
		(error = git_ignore__check_pathspec_for_exact_ignores(
			repo, &ps.pathspec, no_fnmatch)) < 0)
		goto cleanup;

	error = index_apply_to_wd_diff(index, INDEX_ACTION_ADDALL, paths, flags, cb, payload);

	if (error)
		giterr_set_after_callback(error);

cleanup:
	git_iterator_free(wditer);
	git_pathspec__clear(&ps);

	return error;
}
