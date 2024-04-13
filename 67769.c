static void index_entry_adjust_namemask(
		git_index_entry *entry,
		size_t path_length)
{
	entry->flags &= ~GIT_IDXENTRY_NAMEMASK;

	if (path_length < GIT_IDXENTRY_NAMEMASK)
		entry->flags |= path_length & GIT_IDXENTRY_NAMEMASK;
	else
		entry->flags |= GIT_IDXENTRY_NAMEMASK;
}
