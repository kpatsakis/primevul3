static int read_conflict_names(git_index *index, const char *buffer, size_t size)
{
	size_t len;

	/* This gets called multiple times, the vector might already be initialized */
	if (index->names._alloc_size == 0 &&
		git_vector_init(&index->names, 16, conflict_name_cmp) < 0)
		return -1;

#define read_conflict_name(ptr) \
	len = p_strnlen(buffer, size) + 1; \
	if (size < len) { \
		index_error_invalid("reading conflict name entries"); \
		goto out_err; \
	} \
	if (len == 1) \
		ptr = NULL; \
	else { \
		ptr = git__malloc(len); \
		GITERR_CHECK_ALLOC(ptr); \
		memcpy(ptr, buffer, len); \
	} \
	\
	buffer += len; \
	size -= len;

	while (size) {
		git_index_name_entry *conflict_name = git__calloc(1, sizeof(git_index_name_entry));
		GITERR_CHECK_ALLOC(conflict_name);

		read_conflict_name(conflict_name->ancestor);
		read_conflict_name(conflict_name->ours);
		read_conflict_name(conflict_name->theirs);

		if (git_vector_insert(&index->names, conflict_name) < 0)
			goto out_err;

		continue;

out_err:
		git__free(conflict_name->ancestor);
		git__free(conflict_name->ours);
		git__free(conflict_name->theirs);
		git__free(conflict_name);
		return -1;
	}

#undef read_conflict_name

	/* entries are guaranteed to be sorted on-disk */
	git_vector_set_sorted(&index->names, true);

	return 0;
}
