int git_index_write_tree(git_oid *oid, git_index *index)
{
	git_repository *repo;

	assert(oid && index);

	repo = INDEX_OWNER(index);

	if (repo == NULL)
		return create_index_error(-1, "Failed to write tree. "
		  "the index file is not backed up by an existing repository");

	return git_tree__write_index(oid, index, repo);
}
