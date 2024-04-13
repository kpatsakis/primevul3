size_t git_index_reuc_entrycount(git_index *index)
{
	assert(index);
	return index->reuc.length;
}
