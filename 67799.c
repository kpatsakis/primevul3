static int read_reuc(git_index *index, const char *buffer, size_t size)
{
	const char *endptr;
	size_t len;
	int i;

	/* If called multiple times, the vector might already be initialized */
	if (index->reuc._alloc_size == 0 &&
		git_vector_init(&index->reuc, 16, reuc_cmp) < 0)
		return -1;

	while (size) {
		git_index_reuc_entry *lost;

		len = p_strnlen(buffer, size) + 1;
		if (size <= len)
			return index_error_invalid("reading reuc entries");

		lost = reuc_entry_alloc(buffer);
		GITERR_CHECK_ALLOC(lost);

		size -= len;
		buffer += len;

		/* read 3 ASCII octal numbers for stage entries */
		for (i = 0; i < 3; i++) {
			int64_t tmp;

			if (git__strtol64(&tmp, buffer, &endptr, 8) < 0 ||
				!endptr || endptr == buffer || *endptr ||
				tmp < 0 || tmp > UINT32_MAX) {
				index_entry_reuc_free(lost);
				return index_error_invalid("reading reuc entry stage");
			}

			lost->mode[i] = (uint32_t)tmp;

			len = (endptr + 1) - buffer;
			if (size <= len) {
				index_entry_reuc_free(lost);
				return index_error_invalid("reading reuc entry stage");
			}

			size -= len;
			buffer += len;
		}

		/* read up to 3 OIDs for stage entries */
		for (i = 0; i < 3; i++) {
			if (!lost->mode[i])
				continue;
			if (size < 20) {
				index_entry_reuc_free(lost);
				return index_error_invalid("reading reuc entry oid");
			}

			git_oid_fromraw(&lost->oid[i], (const unsigned char *) buffer);
			size -= 20;
			buffer += 20;
		}

		/* entry was read successfully - insert into reuc vector */
		if (git_vector_insert(&index->reuc, lost) < 0)
			return -1;
	}

	/* entries are guaranteed to be sorted on-disk */
	git_vector_set_sorted(&index->reuc, true);

	return 0;
}
