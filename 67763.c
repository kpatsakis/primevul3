static int has_file_name(git_index *index,
	 const git_index_entry *entry, size_t pos, int ok_to_replace)
{
	int retval = 0;
	size_t len = strlen(entry->path);
	int stage = GIT_IDXENTRY_STAGE(entry);
	const char *name = entry->path;

	while (pos < index->entries.length) {
		struct entry_internal *p = index->entries.contents[pos++];

		if (len >= p->pathlen)
			break;
		if (memcmp(name, p->path, len))
			break;
		if (GIT_IDXENTRY_STAGE(&p->entry) != stage)
			continue;
		if (p->path[len] != '/')
			continue;
		retval = -1;
		if (!ok_to_replace)
			break;

		if (index_remove_entry(index, --pos) < 0)
			break;
	}
	return retval;
}
