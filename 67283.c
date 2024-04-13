size_t git_delta_index_size(git_delta_index *index)
{
	assert(index);

	return index->memsize;
}
