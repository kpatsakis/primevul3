static int index_entry_dup(
	git_index_entry **out,
	git_index *index,
	const git_index_entry *src)
{
	if (index_entry_create(out, INDEX_OWNER(index), src->path, false) < 0)
		return -1;

	index_entry_cpy(*out, src);
	return 0;
}
