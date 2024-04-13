size_t git_index_entrycount(const git_index *index)
{
	assert(index);
	return index->entries.length;
}
