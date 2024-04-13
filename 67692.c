static int add_repo_as_submodule(git_index_entry **out, git_index *index, const char *path)
{
	git_repository *sub;
	git_buf abspath = GIT_BUF_INIT;
	git_repository *repo = INDEX_OWNER(index);
	git_reference *head;
	git_index_entry *entry;
	struct stat st;
	int error;

	if (index_entry_create(&entry, INDEX_OWNER(index), path, true) < 0)
		return -1;

	if ((error = git_buf_joinpath(&abspath, git_repository_workdir(repo), path)) < 0)
		return error;

	if ((error = p_stat(abspath.ptr, &st)) < 0) {
		giterr_set(GITERR_OS, "failed to stat repository dir");
		return -1;
	}

	git_index_entry__init_from_stat(entry, &st, !index->distrust_filemode);

	if ((error = git_repository_open(&sub, abspath.ptr)) < 0)
		return error;

	if ((error = git_repository_head(&head, sub)) < 0)
		return error;

	git_oid_cpy(&entry->id, git_reference_target(head));
	entry->mode = GIT_FILEMODE_COMMIT;

	git_reference_free(head);
	git_repository_free(sub);
	git_buf_free(&abspath);

	*out = entry;
	return 0;
}
