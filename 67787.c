static void index_free_deleted(git_index *index)
{
	int readers = (int)git_atomic_get(&index->readers);
	size_t i;

	if (readers > 0 || !index->deleted.length)
		return;

	for (i = 0; i < index->deleted.length; ++i) {
		git_index_entry *ie = git__swap(index->deleted.contents[i], NULL);
		index_entry_free(ie);
	}

	git_vector_clear(&index->deleted);
}
