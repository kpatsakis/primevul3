static int fetch_setup_walk(git_revwalk **out, git_repository *repo)
{
	git_revwalk *walk = NULL;
	git_strarray refs;
	unsigned int i;
	git_reference *ref;
	int error;

	if ((error = git_reference_list(&refs, repo)) < 0)
		return error;

	if ((error = git_revwalk_new(&walk, repo)) < 0)
		return error;

	git_revwalk_sorting(walk, GIT_SORT_TIME);

	for (i = 0; i < refs.count; ++i) {
		/* No tags */
		if (!git__prefixcmp(refs.strings[i], GIT_REFS_TAGS_DIR))
			continue;

		if ((error = git_reference_lookup(&ref, repo, refs.strings[i])) < 0)
			goto on_error;

		if (git_reference_type(ref) == GIT_REF_SYMBOLIC)
			continue;

		if ((error = git_revwalk_push(walk, git_reference_target(ref))) < 0)
			goto on_error;

		git_reference_free(ref);
	}

	git_strarray_free(&refs);
	*out = walk;
	return 0;

on_error:
	git_revwalk_free(walk);
	git_reference_free(ref);
	git_strarray_free(&refs);
	return error;
}
