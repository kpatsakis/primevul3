int git_index_reuc_find(size_t *at_pos, git_index *index, const char *path)
{
	return git_vector_bsearch2(at_pos, &index->reuc, index->reuc_search, path);
}
