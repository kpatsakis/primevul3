static git_index_reuc_entry *reuc_entry_alloc(const char *path)
{
	size_t pathlen = strlen(path),
		structlen = sizeof(struct reuc_entry_internal),
		alloclen;
	struct reuc_entry_internal *entry;

	if (GIT_ADD_SIZET_OVERFLOW(&alloclen, structlen, pathlen) ||
		GIT_ADD_SIZET_OVERFLOW(&alloclen, alloclen, 1))
		return NULL;

	entry = git__calloc(1, alloclen);
	if (!entry)
		return NULL;

	entry->pathlen = pathlen;
	memcpy(entry->path, path, pathlen);
	entry->entry.path = entry->path;

	return (git_index_reuc_entry *)entry;
}
