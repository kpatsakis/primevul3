int git_index_remove_directory(git_index *index, const char *dir, int stage)
{
	git_buf pfx = GIT_BUF_INIT;
	int error = 0;
	size_t pos;
	git_index_entry *entry;

	if (!(error = git_buf_sets(&pfx, dir)) &&
		!(error = git_path_to_dir(&pfx)))
		index_find(&pos, index, pfx.ptr, pfx.size, GIT_INDEX_STAGE_ANY);

	while (!error) {
		entry = git_vector_get(&index->entries, pos);
		if (!entry || git__prefixcmp(entry->path, pfx.ptr) != 0)
			break;

		if (GIT_IDXENTRY_STAGE(entry) != stage) {
			++pos;
			continue;
		}

		error = index_remove_entry(index, pos);

		/* removed entry at 'pos' so we don't need to increment */
	}

	git_buf_free(&pfx);

	return error;
}
