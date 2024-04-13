int git_index_conflict_cleanup(git_index *index)
{
	assert(index);
	return index_conflict_remove(index, NULL);
}
