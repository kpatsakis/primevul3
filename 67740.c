int git_index_remove_all(
	git_index *index,
	const git_strarray *pathspec,
	git_index_matched_path_cb cb,
	void *payload)
{
	int error = index_apply_to_all(
		index, INDEX_ACTION_REMOVE, pathspec, cb, payload);

	if (error) /* make sure error is set if callback stopped iteration */
		giterr_set_after_callback(error);

	return error;
}
