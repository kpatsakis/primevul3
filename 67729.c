const git_index_entry *git_index_get_byindex(
	git_index *index, size_t n)
{
	assert(index);
	git_vector_sort(&index->entries);
	return git_vector_get(&index->entries, n);
}
