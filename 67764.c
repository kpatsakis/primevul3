static int index_apply_to_all(
	git_index *index,
	int action,
	const git_strarray *paths,
	git_index_matched_path_cb cb,
	void *payload)
{
	int error = 0;
	size_t i;
	git_pathspec ps;
	const char *match;
	git_buf path = GIT_BUF_INIT;

	assert(index);

	if ((error = git_pathspec__init(&ps, paths)) < 0)
		return error;

	git_vector_sort(&index->entries);

	for (i = 0; !error && i < index->entries.length; ++i) {
		git_index_entry *entry = git_vector_get(&index->entries, i);

		/* check if path actually matches */
		if (!git_pathspec__match(
				&ps.pathspec, entry->path, false, (bool)index->ignore_case,
				&match, NULL))
			continue;

		/* issue notification callback if requested */
		if (cb && (error = cb(entry->path, match, payload)) != 0) {
			if (error > 0) { /* return > 0 means skip this one */
				error = 0;
				continue;
			}
			if (error < 0)   /* return < 0 means abort */
				break;
		}

		/* index manipulation may alter entry, so don't depend on it */
		if ((error = git_buf_sets(&path, entry->path)) < 0)
			break;

		switch (action) {
		case INDEX_ACTION_NONE:
			break;
		case INDEX_ACTION_UPDATE:
			error = git_index_add_bypath(index, path.ptr);

			if (error == GIT_ENOTFOUND) {
				giterr_clear();

				error = git_index_remove_bypath(index, path.ptr);

				if (!error) /* back up foreach if we removed this */
					i--;
			}
			break;
		case INDEX_ACTION_REMOVE:
			if (!(error = git_index_remove_bypath(index, path.ptr)))
				i--; /* back up foreach if we removed this */
			break;
		default:
			giterr_set(GITERR_INVALID, "unknown index action %d", action);
			error = -1;
			break;
		}
	}

	git_buf_free(&path);
	git_pathspec__clear(&ps);

	return error;
}
