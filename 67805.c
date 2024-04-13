static int truncate_racily_clean(git_index *index)
{
	size_t i;
	int error;
	git_index_entry *entry;
	git_diff_options diff_opts = GIT_DIFF_OPTIONS_INIT;
	git_diff *diff = NULL;
	git_vector paths = GIT_VECTOR_INIT;
	git_diff_delta *delta;

	/* Nothing to do if there's no repo to talk about */
	if (!INDEX_OWNER(index))
		return 0;

	/* If there's no workdir, we can't know where to even check */
	if (!git_repository_workdir(INDEX_OWNER(index)))
		return 0;

	diff_opts.flags |= GIT_DIFF_INCLUDE_TYPECHANGE | GIT_DIFF_IGNORE_SUBMODULES | GIT_DIFF_DISABLE_PATHSPEC_MATCH;
	git_vector_foreach(&index->entries, i, entry) {
		if ((entry->flags_extended & GIT_IDXENTRY_UPTODATE) == 0 &&
			is_racy_entry(index, entry))
			git_vector_insert(&paths, (char *)entry->path);
	}

	if (paths.length == 0)
		goto done;

	diff_opts.pathspec.count = paths.length;
	diff_opts.pathspec.strings = (char **)paths.contents;

	if ((error = git_diff_index_to_workdir(&diff, INDEX_OWNER(index), index, &diff_opts)) < 0)
		return error;

	git_vector_foreach(&diff->deltas, i, delta) {
		entry = (git_index_entry *)git_index_get_bypath(index, delta->old_file.path, 0);

		/* Ensure that we have a stage 0 for this file (ie, it's not a
		 * conflict), otherwise smudging it is quite pointless.
		 */
		if (entry)
			entry->file_size = 0;
	}

done:
	git_diff_free(diff);
	git_vector_free(&paths);
	return 0;
}
