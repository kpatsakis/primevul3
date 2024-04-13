static int index_no_dups(void **old, void *new)
{
	const git_index_entry *entry = new;
	GIT_UNUSED(old);
	giterr_set(GITERR_INDEX, "'%s' appears multiple times at stage %d",
		entry->path, GIT_IDXENTRY_STAGE(entry));
	return GIT_EEXISTS;
}
