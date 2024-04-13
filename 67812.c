static int write_tree_extension(git_index *index, git_filebuf *file)
{
	struct index_extension extension;
	git_buf buf = GIT_BUF_INIT;
	int error;

	if (index->tree == NULL)
		return 0;

	if ((error = git_tree_cache_write(&buf, index->tree)) < 0)
		return error;

	memset(&extension, 0x0, sizeof(struct index_extension));
	memcpy(&extension.signature, INDEX_EXT_TREECACHE_SIG, 4);
	extension.extension_size = (uint32_t)buf.size;

	error = write_extension(file, &extension, &buf);

	git_buf_free(&buf);

	return error;
}
