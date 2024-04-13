int git_index_conflict_add(git_index *index,
	const git_index_entry *ancestor_entry,
	const git_index_entry *our_entry,
	const git_index_entry *their_entry)
{
	git_index_entry *entries[3] = { 0 };
	unsigned short i;
	int ret = 0;

	assert (index);

	if ((ancestor_entry &&
			(ret = index_entry_dup(&entries[0], index, ancestor_entry)) < 0) ||
		(our_entry &&
			(ret = index_entry_dup(&entries[1], index, our_entry)) < 0) ||
		(their_entry &&
			(ret = index_entry_dup(&entries[2], index, their_entry)) < 0))
		goto on_error;

	/* Validate entries */
	for (i = 0; i < 3; i++) {
		if (entries[i] && !valid_filemode(entries[i]->mode)) {
			giterr_set(GITERR_INDEX, "invalid filemode for stage %d entry",
				i + 1);
			return -1;
		}
	}

	/* Remove existing index entries for each path */
	for (i = 0; i < 3; i++) {
		if (entries[i] == NULL)
			continue;

		if ((ret = git_index_remove(index, entries[i]->path, 0)) != 0) {
			if (ret != GIT_ENOTFOUND)
				goto on_error;

			giterr_clear();
			ret = 0;
		}
	}

	/* Add the conflict entries */
	for (i = 0; i < 3; i++) {
		if (entries[i] == NULL)
			continue;

		/* Make sure stage is correct */
		GIT_IDXENTRY_STAGE_SET(entries[i], i + 1);

		if ((ret = index_insert(index, &entries[i], 1, true, true, false)) < 0)
			goto on_error;

		entries[i] = NULL; /* don't free if later entry fails */
	}

	return 0;

on_error:
	for (i = 0; i < 3; i++) {
		if (entries[i] != NULL)
			index_entry_free(entries[i]);
	}

	return ret;
}
