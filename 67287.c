static int lookup_index_alloc(
	void **out, unsigned long *out_len, size_t entries, size_t hash_count)
{
	size_t entries_len, hash_len, index_len;

	GITERR_CHECK_ALLOC_MULTIPLY(&entries_len, entries, sizeof(struct index_entry));
	GITERR_CHECK_ALLOC_MULTIPLY(&hash_len, hash_count, sizeof(struct index_entry *));

	GITERR_CHECK_ALLOC_ADD(&index_len, sizeof(struct git_delta_index), entries_len);
	GITERR_CHECK_ALLOC_ADD(&index_len, index_len, hash_len);

	if (!git__is_ulong(index_len)) {
		giterr_set(GITERR_NOMEMORY, "overly large delta");
		return -1;
	}

	*out = git__malloc(index_len);
	GITERR_CHECK_ALLOC(*out);

	*out_len = index_len;
	return 0;
}
