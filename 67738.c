static int git_index_read_iterator(
	git_index *index,
	git_iterator *new_iterator,
	size_t new_length_hint)
{
	git_vector new_entries = GIT_VECTOR_INIT,
		remove_entries = GIT_VECTOR_INIT;
	git_idxmap *new_entries_map = NULL;
	git_iterator *index_iterator = NULL;
	git_iterator_options opts = GIT_ITERATOR_OPTIONS_INIT;
	const git_index_entry *old_entry, *new_entry;
	git_index_entry *entry;
	size_t i;
	int error;

	assert((new_iterator->flags & GIT_ITERATOR_DONT_IGNORE_CASE));

	if ((error = git_vector_init(&new_entries, new_length_hint, index->entries._cmp)) < 0 ||
		(error = git_vector_init(&remove_entries, index->entries.length, NULL)) < 0 ||
		(error = git_idxmap_alloc(&new_entries_map)) < 0)
		goto done;

	if (index->ignore_case && new_length_hint)
		git_idxmap_icase_resize((khash_t(idxicase) *) new_entries_map, new_length_hint);
	else if (new_length_hint)
		git_idxmap_resize(new_entries_map, new_length_hint);

	opts.flags = GIT_ITERATOR_DONT_IGNORE_CASE |
		GIT_ITERATOR_INCLUDE_CONFLICTS;

	if ((error = git_iterator_for_index(&index_iterator,
			git_index_owner(index), index, &opts)) < 0 ||
		((error = git_iterator_current(&old_entry, index_iterator)) < 0 &&
			error != GIT_ITEROVER) ||
		((error = git_iterator_current(&new_entry, new_iterator)) < 0 &&
			error != GIT_ITEROVER))
		goto done;

	while (true) {
		git_index_entry
			*dup_entry = NULL,
			*add_entry = NULL,
			*remove_entry = NULL;
		int diff;

		error = 0;

		if (old_entry && new_entry)
			diff = git_index_entry_cmp(old_entry, new_entry);
		else if (!old_entry && new_entry)
			diff = 1;
		else if (old_entry && !new_entry)
			diff = -1;
		else
			break;

		if (diff < 0) {
			remove_entry = (git_index_entry *)old_entry;
		} else if (diff > 0) {
			dup_entry = (git_index_entry *)new_entry;
		} else {
			/* Path and stage are equal, if the OID is equal, keep it to
			 * keep the stat cache data.
			 */
			if (git_oid_equal(&old_entry->id, &new_entry->id) &&
				old_entry->mode == new_entry->mode) {
				add_entry = (git_index_entry *)old_entry;
			} else {
				dup_entry = (git_index_entry *)new_entry;
				remove_entry = (git_index_entry *)old_entry;
			}
		}

		if (dup_entry) {
			if ((error = index_entry_dup_nocache(&add_entry, index, dup_entry)) < 0)
				goto done;

			index_entry_adjust_namemask(add_entry,
				((struct entry_internal *)add_entry)->pathlen);
		}

		/* invalidate this path in the tree cache if this is new (to
		 * invalidate the parent trees)
		 */
		if (dup_entry && !remove_entry && index->tree)
			git_tree_cache_invalidate_path(index->tree, dup_entry->path);

		if (add_entry) {
			if ((error = git_vector_insert(&new_entries, add_entry)) == 0)
				INSERT_IN_MAP_EX(index, new_entries_map, add_entry, &error);
		}

		if (remove_entry && error >= 0)
			error = git_vector_insert(&remove_entries, remove_entry);

		if (error < 0) {
			giterr_set(GITERR_INDEX, "failed to insert entry");
			goto done;
		}

		if (diff <= 0) {
			if ((error = git_iterator_advance(&old_entry, index_iterator)) < 0 &&
				error != GIT_ITEROVER)
				goto done;
		}

		if (diff >= 0) {
			if ((error = git_iterator_advance(&new_entry, new_iterator)) < 0 &&
				error != GIT_ITEROVER)
				goto done;
		}
	}

	git_index_name_clear(index);
	git_index_reuc_clear(index);

	git_vector_swap(&new_entries, &index->entries);
	new_entries_map = git__swap(index->entries_map, new_entries_map);

	git_vector_foreach(&remove_entries, i, entry) {
		if (index->tree)
			git_tree_cache_invalidate_path(index->tree, entry->path);

		index_entry_free(entry);
	}

	clear_uptodate(index);

	error = 0;

done:
	git_idxmap_free(new_entries_map);
	git_vector_free(&new_entries);
	git_vector_free(&remove_entries);
	git_iterator_free(index_iterator);
	return error;
}
