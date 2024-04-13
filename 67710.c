const git_oid *git_index_checksum(git_index *index)
{
	return &index->checksum;
}
