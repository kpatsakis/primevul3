int git_index_caps(const git_index *index)
{
	return ((index->ignore_case ? GIT_INDEXCAP_IGNORE_CASE : 0) |
			(index->distrust_filemode ? GIT_INDEXCAP_NO_FILEMODE : 0) |
			(index->no_symlinks ? GIT_INDEXCAP_NO_SYMLINKS : 0));
}
