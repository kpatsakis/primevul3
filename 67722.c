int git_index_entry_is_conflict(const git_index_entry *entry)
{
	return (GIT_IDXENTRY_STAGE(entry) > 0);
}
