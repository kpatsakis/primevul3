int git_index_write_tree_to(
	git_oid *oid, git_index *index, git_repository *repo)
{
	assert(oid && index && repo);
	return git_tree__write_index(oid, index, repo);
}
