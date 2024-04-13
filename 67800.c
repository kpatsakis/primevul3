static int reuc_cmp(const void *a, const void *b)
{
	const git_index_reuc_entry *info_a = a;
	const git_index_reuc_entry *info_b = b;

	return strcmp(info_a->path, info_b->path);
}
