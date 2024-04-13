static int index_conflict_remove(git_index *index, const char *path)
{
	size_t pos = 0;
	git_index_entry *conflict_entry;
	int error = 0;

	if (path != NULL && git_index_find(&pos, index, path) < 0)
		return GIT_ENOTFOUND;

	while ((conflict_entry = git_vector_get(&index->entries, pos)) != NULL) {

		if (path != NULL &&
			index->entries_cmp_path(conflict_entry->path, path) != 0)
			break;

		if (GIT_IDXENTRY_STAGE(conflict_entry) == 0) {
			pos++;
			continue;
		}

		if ((error = index_remove_entry(index, pos)) < 0)
			break;
	}

	return error;
}
