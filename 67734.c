const git_index_name_entry *git_index_name_get_byindex(
	git_index *index, size_t n)
{
	assert(index);

	git_vector_sort(&index->names);
	return git_vector_get(&index->names, n);
}
