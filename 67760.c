int git_indexwriter_commit(git_indexwriter *writer)
{
	int error;
	git_oid checksum = {{ 0 }};

	if (!writer->should_write)
		return 0;

	git_vector_sort(&writer->index->entries);
	git_vector_sort(&writer->index->reuc);

	if ((error = write_index(&checksum, writer->index, &writer->file)) < 0) {
		git_indexwriter_cleanup(writer);
		return error;
	}

	if ((error = git_filebuf_commit(&writer->file)) < 0)
		return error;

	if ((error = git_futils_filestamp_check(
		&writer->index->stamp, writer->index->index_file_path)) < 0) {
		giterr_set(GITERR_OS, "could not read index timestamp");
		return -1;
	}

	writer->index->on_disk = 1;
	git_oid_cpy(&writer->index->checksum, &checksum);

	git_index_free(writer->index);
	writer->index = NULL;

	return 0;
}
