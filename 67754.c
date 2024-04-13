int git_index_update_all(
	git_index *index,
	const git_strarray *pathspec,
	git_index_matched_path_cb cb,
	void *payload)
{
	int error = index_apply_to_wd_diff(index, INDEX_ACTION_UPDATE, pathspec, 0, cb, payload);
	if (error) /* make sure error is set if callback stopped iteration */
		giterr_set_after_callback(error);

	return error;
}
