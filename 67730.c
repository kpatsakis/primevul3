const git_index_entry *git_index_get_bypath(
	git_index *index, const char *path, int stage)
{
	khiter_t pos;
	git_index_entry key = {{ 0 }};

	assert(index);

	key.path = path;
	GIT_IDXENTRY_STAGE_SET(&key, stage);

	LOOKUP_IN_MAP(pos, index, &key);

	if (git_idxmap_valid_index(index->entries_map, pos))
		return git_idxmap_value_at(index->entries_map, pos);

	giterr_set(GITERR_INDEX, "index does not contain '%s'", path);
	return NULL;
}
