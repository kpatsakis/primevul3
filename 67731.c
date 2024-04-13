int git_index_has_conflicts(const git_index *index)
{
	size_t i;
	git_index_entry *entry;

	assert(index);

	git_vector_foreach(&index->entries, i, entry) {
		if (GIT_IDXENTRY_STAGE(entry) > 0)
			return 1;
	}

	return 0;
}
