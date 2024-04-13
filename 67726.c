int git_index_find(size_t *at_pos, git_index *index, const char *path)
{
	size_t pos;

	assert(index && path);

	if (git_vector_bsearch2(
			&pos, &index->entries, index->entries_search_path, path) < 0) {
		giterr_set(GITERR_INDEX, "index does not contain %s", path);
		return GIT_ENOTFOUND;
	}

	/* Since our binary search only looked at path, we may be in the
	 * middle of a list of stages.
	 */
	for (; pos > 0; --pos) {
		const git_index_entry *prev = git_vector_get(&index->entries, pos - 1);

		if (index->entries_cmp_path(prev->path, path) != 0)
			break;
	}

	if (at_pos)
		*at_pos = pos;

	return 0;
}
