static void clear_uptodate(git_index *index)
{
	git_index_entry *entry;
	size_t i;

	git_vector_foreach(&index->entries, i, entry)
		entry->flags_extended &= ~GIT_IDXENTRY_UPTODATE;
}
