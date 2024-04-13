static int write_name_extension(git_index *index, git_filebuf *file)
{
	git_buf name_buf = GIT_BUF_INIT;
	git_vector *out = &index->names;
	git_index_name_entry *conflict_name;
	struct index_extension extension;
	size_t i;
	int error = 0;

	git_vector_foreach(out, i, conflict_name) {
		if ((error = create_name_extension_data(&name_buf, conflict_name)) < 0)
			goto done;
	}

	memset(&extension, 0x0, sizeof(struct index_extension));
	memcpy(&extension.signature, INDEX_EXT_CONFLICT_NAME_SIG, 4);
	extension.extension_size = (uint32_t)name_buf.size;

	error = write_extension(file, &extension, &name_buf);

	git_buf_free(&name_buf);

done:
	return error;
}
