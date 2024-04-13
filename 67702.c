int git_index__fill(git_index *index, const git_vector *source_entries)
{
	const git_index_entry *source_entry = NULL;
	size_t i;
	int ret = 0;

	assert(index);

	if (!source_entries->length)
		return 0;

	git_vector_size_hint(&index->entries, source_entries->length);
	git_idxmap_resize(index->entries_map, (khint_t)(source_entries->length * 1.3));

	git_vector_foreach(source_entries, i, source_entry) {
		git_index_entry *entry = NULL;

		if ((ret = index_entry_dup(&entry, index, source_entry)) < 0)
			break;

		index_entry_adjust_namemask(entry, ((struct entry_internal *)entry)->pathlen);
		entry->flags_extended |= GIT_IDXENTRY_UPTODATE;
		entry->mode = git_index__create_mode(entry->mode);

		if ((ret = git_vector_insert(&index->entries, entry)) < 0)
			break;

		INSERT_IN_MAP(index, entry, &ret);
		if (ret < 0)
			break;
	}

	if (!ret)
		git_vector_sort(&index->entries);

	return ret;
}
