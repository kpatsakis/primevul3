static unsigned int index_merge_mode(
	git_index *index, git_index_entry *existing, unsigned int mode)
{
	if (index->no_symlinks && S_ISREG(mode) &&
		existing && S_ISLNK(existing->mode))
		return existing->mode;

	if (index->distrust_filemode && S_ISREG(mode))
		return (existing && S_ISREG(existing->mode)) ?
			existing->mode : git_index__create_mode(0666);

	return git_index__create_mode(mode);
}
