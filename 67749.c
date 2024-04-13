int git_index_set_caps(git_index *index, int caps)
{
	unsigned int old_ignore_case;

	assert(index);

	old_ignore_case = index->ignore_case;

	if (caps == GIT_INDEXCAP_FROM_OWNER) {
		git_repository *repo = INDEX_OWNER(index);
		int val;

		if (!repo)
			return create_index_error(
				-1, "cannot access repository to set index caps");

		if (!git_repository__cvar(&val, repo, GIT_CVAR_IGNORECASE))
			index->ignore_case = (val != 0);
		if (!git_repository__cvar(&val, repo, GIT_CVAR_FILEMODE))
			index->distrust_filemode = (val == 0);
		if (!git_repository__cvar(&val, repo, GIT_CVAR_SYMLINKS))
			index->no_symlinks = (val == 0);
	}
	else {
		index->ignore_case = ((caps & GIT_INDEXCAP_IGNORE_CASE) != 0);
		index->distrust_filemode = ((caps & GIT_INDEXCAP_NO_FILEMODE) != 0);
		index->no_symlinks = ((caps & GIT_INDEXCAP_NO_SYMLINKS) != 0);
	}

	if (old_ignore_case != index->ignore_case) {
		git_index__set_ignore_case(index, (bool)index->ignore_case);
	}

	return 0;
}
