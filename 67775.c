static void index_entry_free(git_index_entry *entry)
{
	if (!entry)
		return;

	memset(&entry->id, 0, sizeof(entry->id));
	git__free(entry);
}
