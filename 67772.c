static int index_entry_create(
	git_index_entry **out,
	git_repository *repo,
	const char *path,
	bool from_workdir)
{
	size_t pathlen = strlen(path), alloclen;
	struct entry_internal *entry;
	unsigned int path_valid_flags = GIT_PATH_REJECT_INDEX_DEFAULTS;

	/* always reject placing `.git` in the index and directory traversal.
	 * when requested, disallow platform-specific filenames and upgrade to
	 * the platform-specific `.git` tests (eg, `git~1`, etc).
	 */
	if (from_workdir)
		path_valid_flags |= GIT_PATH_REJECT_WORKDIR_DEFAULTS;

	if (!git_path_isvalid(repo, path, path_valid_flags)) {
		giterr_set(GITERR_INDEX, "invalid path: '%s'", path);
		return -1;
	}

	GITERR_CHECK_ALLOC_ADD(&alloclen, sizeof(struct entry_internal), pathlen);
	GITERR_CHECK_ALLOC_ADD(&alloclen, alloclen, 1);
	entry = git__calloc(1, alloclen);
	GITERR_CHECK_ALLOC(entry);

	entry->pathlen = pathlen;
	memcpy(entry->path, path, pathlen);
	entry->entry.path = entry->path;

	*out = (git_index_entry *)entry;
	return 0;
}
