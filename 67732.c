int git_index_name_add(git_index *index,
	const char *ancestor, const char *ours, const char *theirs)
{
	git_index_name_entry *conflict_name;

	assert((ancestor && ours) || (ancestor && theirs) || (ours && theirs));

	conflict_name = git__calloc(1, sizeof(git_index_name_entry));
	GITERR_CHECK_ALLOC(conflict_name);

	if ((ancestor && !(conflict_name->ancestor = git__strdup(ancestor))) ||
		(ours     && !(conflict_name->ours     = git__strdup(ours))) ||
		(theirs   && !(conflict_name->theirs   = git__strdup(theirs))) ||
		git_vector_insert(&index->names, conflict_name) < 0)
	{
		index_name_entry_free(conflict_name);
		return -1;
	}

	return 0;
}
