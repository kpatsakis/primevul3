int git_indexwriter_init(
	git_indexwriter *writer,
	git_index *index)
{
	int error;

	GIT_REFCOUNT_INC(index);

	writer->index = index;

	if (!index->index_file_path)
		return create_index_error(-1,
			"failed to write index: The index is in-memory only");

	if ((error = git_filebuf_open(
		&writer->file, index->index_file_path, GIT_FILEBUF_HASH_CONTENTS, GIT_INDEX_FILE_MODE)) < 0) {

		if (error == GIT_ELOCKED)
			giterr_set(GITERR_INDEX, "the index is locked; this might be due to a concurrent or crashed process");

		return error;
	}

	writer->should_write = 1;

	return 0;
}
