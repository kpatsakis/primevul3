static int index_insert(
	git_index *index,
	git_index_entry **entry_ptr,
	int replace,
	bool trust_path,
	bool trust_mode,
	bool trust_id)
{
	int error = 0;
	size_t path_length, position;
	git_index_entry *existing, *best, *entry;

	assert(index && entry_ptr);

	entry = *entry_ptr;

	/* make sure that the path length flag is correct */
	path_length = ((struct entry_internal *)entry)->pathlen;
	index_entry_adjust_namemask(entry, path_length);

	/* this entry is now up-to-date and should not be checked for raciness */
	entry->flags_extended |= GIT_IDXENTRY_UPTODATE;

	git_vector_sort(&index->entries);

	/* look if an entry with this path already exists, either staged, or (if
	 * this entry is a regular staged item) as the "ours" side of a conflict.
	 */
	index_existing_and_best(&existing, &position, &best, index, entry);

	/* update the file mode */
	entry->mode = trust_mode ?
		git_index__create_mode(entry->mode) :
		index_merge_mode(index, best, entry->mode);

	/* canonicalize the directory name */
	if (!trust_path)
		error = canonicalize_directory_path(index, entry, best);

	/* ensure that the given id exists (unless it's a submodule) */
	if (!error && !trust_id && INDEX_OWNER(index) &&
		(entry->mode & GIT_FILEMODE_COMMIT) != GIT_FILEMODE_COMMIT) {

		if (!git_object__is_valid(INDEX_OWNER(index), &entry->id,
			git_object__type_from_filemode(entry->mode)))
			error = -1;
	}

	/* look for tree / blob name collisions, removing conflicts if requested */
	if (!error)
		error = check_file_directory_collision(index, entry, position, replace);

	if (error < 0)
		/* skip changes */;

	/* if we are replacing an existing item, overwrite the existing entry
	 * and return it in place of the passed in one.
	 */
	else if (existing) {
		if (replace) {
			index_entry_cpy(existing, entry);

			if (trust_path)
				memcpy((char *)existing->path, entry->path, strlen(entry->path));
		}

		index_entry_free(entry);
		*entry_ptr = entry = existing;
	}
	else {
		/* if replace is not requested or no existing entry exists, insert
		 * at the sorted position.  (Since we re-sort after each insert to
		 * check for dups, this is actually cheaper in the long run.)
		 */
		error = git_vector_insert_sorted(&index->entries, entry, index_no_dups);

		if (error == 0) {
			INSERT_IN_MAP(index, entry, &error);
		}
	}

	if (error < 0) {
		index_entry_free(*entry_ptr);
		*entry_ptr = NULL;
	}

	return error;
}
