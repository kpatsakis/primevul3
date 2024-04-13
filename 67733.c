void git_index_name_clear(git_index *index)
{
	size_t i;
	git_index_name_entry *conflict_name;

	assert(index);

	git_vector_foreach(&index->names, i, conflict_name)
		index_name_entry_free(conflict_name);

	git_vector_clear(&index->names);
}
