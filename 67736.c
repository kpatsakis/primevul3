const char * git_index_path(const git_index *index)
{
	assert(index);
	return index->index_file_path;
}
