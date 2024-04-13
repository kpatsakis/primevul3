int git_index_conflict_remove(git_index *index, const char *path)
{
	assert(index && path);
	return index_conflict_remove(index, path);
}
