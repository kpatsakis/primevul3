int git_index_add_bypath(git_index *index, const char *path)
{
	git_index_entry *entry = NULL;
	int ret;

	assert(index && path);

	if ((ret = index_entry_init(&entry, index, path)) == 0)
		ret = index_insert(index, &entry, 1, false, false, true);

	/* If we were given a directory, let's see if it's a submodule */
	if (ret < 0 && ret != GIT_EDIRECTORY)
		return ret;

	if (ret == GIT_EDIRECTORY) {
		git_submodule *sm;
		git_error_state err;

		giterr_state_capture(&err, ret);

		ret = git_submodule_lookup(&sm, INDEX_OWNER(index), path);
		if (ret == GIT_ENOTFOUND)
			return giterr_state_restore(&err);

		giterr_state_free(&err);

		/*
		 * EEXISTS means that there is a repository at that path, but it's not known
		 * as a submodule. We add its HEAD as an entry and don't register it.
		 */
		if (ret == GIT_EEXISTS) {
			if ((ret = add_repo_as_submodule(&entry, index, path)) < 0)
				return ret;

			if ((ret = index_insert(index, &entry, 1, false, false, true)) < 0)
				return ret;
		} else if (ret < 0) {
			return ret;
		} else {
			ret = git_submodule_add_to_index(sm, false);
			git_submodule_free(sm);
			return ret;
		}
	}

	/* Adding implies conflict was resolved, move conflict entries to REUC */
	if ((ret = index_conflict_to_reuc(index, path)) < 0 && ret != GIT_ENOTFOUND)
		return ret;

	git_tree_cache_invalidate_path(index->tree, entry->path);
	return 0;
}
