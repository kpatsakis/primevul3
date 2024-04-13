unsigned git_index_version(git_index *index)
{
	assert(index);

	return index->version;
}
