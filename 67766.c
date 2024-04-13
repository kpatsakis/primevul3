static int index_conflict__get_byindex(
	const git_index_entry **ancestor_out,
	const git_index_entry **our_out,
	const git_index_entry **their_out,
	git_index *index,
	size_t n)
{
	const git_index_entry *conflict_entry;
	const char *path = NULL;
	size_t count;
	int stage, len = 0;

	assert(ancestor_out && our_out && their_out && index);

	*ancestor_out = NULL;
	*our_out = NULL;
	*their_out = NULL;

	for (count = git_index_entrycount(index); n < count; ++n) {
		conflict_entry = git_vector_get(&index->entries, n);

		if (path && index->entries_cmp_path(conflict_entry->path, path) != 0)
			break;

		stage = GIT_IDXENTRY_STAGE(conflict_entry);
		path = conflict_entry->path;

		switch (stage) {
		case 3:
			*their_out = conflict_entry;
			len++;
			break;
		case 2:
			*our_out = conflict_entry;
			len++;
			break;
		case 1:
			*ancestor_out = conflict_entry;
			len++;
			break;
		default:
			break;
		};
	}

	return len;
}
