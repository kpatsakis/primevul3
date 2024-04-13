static int index_apply_to_wd_diff(git_index *index, int action, const git_strarray *paths,
				  unsigned int flags,
				  git_index_matched_path_cb cb, void *payload)
{
	int error;
	git_diff *diff;
	git_pathspec ps;
	git_repository *repo;
	git_diff_options opts = GIT_DIFF_OPTIONS_INIT;
	struct foreach_diff_data data = {
		index,
		NULL,
		flags,
		cb,
		payload,
	};

	assert(index);
	assert(action == INDEX_ACTION_UPDATE || action == INDEX_ACTION_ADDALL);

	repo = INDEX_OWNER(index);

	if (!repo) {
		return create_index_error(-1,
			"cannot run update; the index is not backed up by a repository.");
	}

	/*
	 * We do the matching ourselves intead of passing the list to
	 * diff because we want to tell the callback which one
	 * matched, which we do not know if we ask diff to filter for us.
	 */
	if ((error = git_pathspec__init(&ps, paths)) < 0)
		return error;

	opts.flags = GIT_DIFF_INCLUDE_TYPECHANGE;
	if (action == INDEX_ACTION_ADDALL) {
		opts.flags |= GIT_DIFF_INCLUDE_UNTRACKED |
			GIT_DIFF_RECURSE_UNTRACKED_DIRS;

		if (flags == GIT_INDEX_ADD_FORCE)
			opts.flags |= GIT_DIFF_INCLUDE_IGNORED;
	}

	if ((error = git_diff_index_to_workdir(&diff, repo, index, &opts)) < 0)
		goto cleanup;

	data.pathspec = &ps;
	error = git_diff_foreach(diff, apply_each_file, NULL, NULL, NULL, &data);
	git_diff_free(diff);

	if (error) /* make sure error is set if callback stopped iteration */
		giterr_set_after_callback(error);

cleanup:
	git_pathspec__clear(&ps);
	return error;
}
