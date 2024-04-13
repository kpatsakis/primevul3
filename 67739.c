int git_index_remove(git_index *index, const char *path, int stage)
{
	int error;
	size_t position;
	git_index_entry remove_key = {{ 0 }};

	remove_key.path = path;
	GIT_IDXENTRY_STAGE_SET(&remove_key, stage);

	DELETE_IN_MAP(index, &remove_key);

	if (index_find(&position, index, path, 0, stage) < 0) {
		giterr_set(
			GITERR_INDEX, "index does not contain %s at stage %d", path, stage);
		error = GIT_ENOTFOUND;
	} else {
		error = index_remove_entry(index, position);
	}

	return error;
}
