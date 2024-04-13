static int index_reuc_on_dup(void **old, void *new)
{
	index_entry_reuc_free(*old);
	*old = new;
	return GIT_EEXISTS;
}
